#include <stdio.h>
#include <PsString.h>
#include <PsThread.h>

#include "PhysXInternal.h"
#include "PhysXUtils.h"
#include "PhysicsMaterial.h"
#include "PhysicsShape.h"

#include "asset/AssetsManager.h"
#include "scene/Components.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "PhysXInternal"

namespace pio
{
	class PhysxErrorCallback : public physx::PxErrorCallback
	{
	public:
		PhysxErrorCallback() {}
		virtual ~PhysxErrorCallback() = default;

		virtual void reportError(physx::PxErrorCode::Enum code, const char *message, const char *file, int line) override
		{
			const char *errorCode{ nullptr };

			switch (code)
			{
				case physx::PxErrorCode::eNO_ERROR:
					errorCode = "no error";
					break;
				case physx::PxErrorCode::eINVALID_PARAMETER:
					errorCode = "invalid parameter";
					break;
				case physx::PxErrorCode::eINVALID_OPERATION:
					errorCode = "invalid operation";
					break;
				case physx::PxErrorCode::eOUT_OF_MEMORY:
					errorCode = "out of memory";
					break;
				case physx::PxErrorCode::eDEBUG_INFO:
					errorCode = "info";
					break;
				case physx::PxErrorCode::eDEBUG_WARNING:
					errorCode = "warning";
					break;
				case physx::PxErrorCode::ePERF_WARNING:
					errorCode = "performance warning";
					break;
				case physx::PxErrorCode::eABORT:
					errorCode = "abort";
					break;
				case physx::PxErrorCode::eINTERNAL_ERROR:
					errorCode = "internal error";
					break;
				case physx::PxErrorCode::eMASK_ALL:
				default:
					errorCode = "unknown error";
					break;
			}

			if (errorCode)
			{
				char buffer[1024];
				sprintf(buffer, "%s (%d) : %s : %s\n", file, line, errorCode, message);

				// in release builds we also want to halt execution 
				// and make sure that the error message is flushed  
				while (code == physx::PxErrorCode::eABORT)
				{
					LOGE("%s", buffer);
					physx::shdfnd::Thread::sleep(1000);
				}
			}
		}
	};

	PIO_SINGLETON_IMPL(PhysXInternal)

	static PhysxErrorCallback g_errCallback{};
	static physx::PxDefaultAllocator g_allocator{};
	static physx::PxFoundation *g_foundation{ nullptr };
	static physx::PxPvd *g_pvd{ nullptr };
	static physx::PxPhysics *g_physics{ nullptr };
	static physx::PxDefaultCpuDispatcher *g_dispatcher{ nullptr };

	PhysXInternal::PhysXInternal()
	{
		if (!g_foundation)
		{
			g_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, g_allocator, g_errCallback);

			g_pvd = physx::PxCreatePvd(*g_foundation);
			physx::PxPvdTransport *transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
			g_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

			g_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *g_foundation, physx::PxTolerancesScale(), true, g_pvd);
			// in current implementation, all scenes use the default dispatcher
			g_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);

			LOGD("physx init!");
		}
	}

	PhysXInternal::~PhysXInternal()
	{
		LOGD("physx shutdown");
		PHYSX_RELEASE(g_dispatcher);
		PHYSX_RELEASE(g_physics);
		if (g_pvd)
		{
			physx::PxPvdTransport *transport = g_pvd->getTransport();
			PHYSX_RELEASE(g_pvd);
			PHYSX_RELEASE(transport);
		}
		PHYSX_RELEASE(g_foundation);
	}

	void PhysXInternal::Init()
	{
		PhysXInternal::Get();
	}

	void PhysXInternal::Shutdown()
	{
		std::lock_guard<std::mutex> lk{ s_mutex };
		delete s_instance;
		s_instance = nullptr;
	}

	physx::PxScene *PhysXInternal::createScene(const PhysicsSceneDesc &spec)
	{
		physx::PxSceneDesc sceneDesc(g_physics->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
		sceneDesc.cpuDispatcher = g_dispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		physx::PxScene *scene = g_physics->createScene(sceneDesc);
		if (scene)
		{
			physx::PxPvdSceneClient *pvdClient = scene->getScenePvdClient();
			if (pvdClient)
			{
				pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
				pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
				pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
			}
		}
		else
		{
			LOGE("fail to create physx scene");
		}
		return scene;
	}

	physx::PxRigidActor *PhysXInternal::createActor(const RigidBodyComponent &comp)
	{
		physx::PxRigidActor *actor{ nullptr };
		if (comp.BodyType == RigidBodyComponent::Type::Static)
		{
			actor = g_physics->createRigidStatic(physx::PxTransform(PHYSX_VEC3(comp.Center)));
		}
		else if (comp.BodyType == RigidBodyComponent::Type::Dynamic)
		{
			actor = g_physics->createRigidDynamic(physx::PxTransform(PHYSX_VEC3(comp.Center)));
			physx::PxRigidBodyExt::updateMassAndInertia(*((physx::PxRigidDynamic *)actor), PIO_NORMAL_DENSITY);
		}

		if (actor)
		{
			actor->setName(comp.Name.data());
		}
		else
		{
			LOGE("fail to create actor, rigid body type[%u], name[%s]", comp.BodyType, comp.Name.data());
		}

		return actor;
	}

	physx::PxMaterial *PhysXInternal::createMaterial(const Ref<PhysicsMaterial> &mat)
	{
		return g_physics->createMaterial(mat->StaticFriction, mat->DynamicFriction, mat->Bounciness);
	}

	physx::PxShape *PhysXInternal::createShape(ColliderShape *shape, PhysicsMaterial *mat)
	{
		physx::PxShape *result{ nullptr };
		if (!shape || !mat)
		{
			LOGE("err! invalid input");
			return result;
		}

		if (shape->is<BoxColliderShape>())
		{
			auto *p = shape->as<BoxColliderShape>();
			result = g_physics->createShape(physx::PxBoxGeometry(PHYSX_VEC3(p->m_halfSize)), 
											*mat->getMaterial(), 
											false, 
											GetShapeFlag(shape->m_flags));
		}

		return result;
	}
}