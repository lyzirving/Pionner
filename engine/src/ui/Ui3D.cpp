#include "Ui3D.h"

#include "asset/AssetsManager.h"

#include "scene/Registry.h"
#include "scene/Components.h"

#include "gfx/struct/Geometry.h"
#include "gfx/struct/MeshFactory.h"

#include "physics/PhysicsSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Ui3D"

// Length extension that is used to enlarge touch volume
#define UI3D_TOUCH_EXT (3.5f)

namespace pio
{
	UiCoordinate3D::UiCoordinate3D(const UiCoordinate3DBuilder &builder) : Shape(builder.Shape), Builder(builder)
	{
		XAxisEnt = Registry::Get()->create<C3dUIComponent, TransformComponent, BoxColliderComponent>();
		XAxisEnt->getComponent<BoxColliderComponent>().ShapeFlags = PIO_BIT(PhysicsShapeFlag::Visualize) | PIO_BIT(PhysicsShapeFlag::Query) | PIO_BIT(PhysicsShapeFlag::Trigger);

		YAxisEnt = Registry::Get()->create<C3dUIComponent, TransformComponent, BoxColliderComponent>();
		YAxisEnt->getComponent<BoxColliderComponent>().ShapeFlags = PIO_BIT(PhysicsShapeFlag::Visualize) | PIO_BIT(PhysicsShapeFlag::Query) | PIO_BIT(PhysicsShapeFlag::Trigger);

		ZAxisEnt = Registry::Get()->create<C3dUIComponent, TransformComponent, BoxColliderComponent>();
		ZAxisEnt->getComponent<BoxColliderComponent>().ShapeFlags = PIO_BIT(PhysicsShapeFlag::Visualize) | PIO_BIT(PhysicsShapeFlag::Query) | PIO_BIT(PhysicsShapeFlag::Trigger);

		build(builder);
	}

	void UiCoordinate3D::build(const UiCoordinate3DBuilder &builder)
	{
		switch (builder.Shape)
		{
			case CoordinateShape::Cylinder:
			{
				buildImpl(builder.CylinderInfo);
				break;
			}
			case CoordinateShape::Arrow:
			{
				buildImpl(builder.ArrowInfo);
				break;
			}
			default:
				break;
		}
	}

	void UiCoordinate3D::buildImpl(const CylinderConfig &config)
	{
		//X axis
		{
			Ref<MeshSource> meshSrc = MeshFactory::CreateCylinder(config.Radius, config.Height, config.Itr);
			Ref<Asset> meshAsset = AssetsManager::CreateRuntimeAssets<StaticMesh>(meshSrc);

			Ref<Cylinder> cylinder = RefCast<MeshSource, Cylinder>(meshSrc);
			cylinder->setColor(glm::vec3(1.f, 0.f, 0.f));

			std::vector<Submesh> &submeshes = const_cast<std::vector<Submesh> &>(meshSrc->getSubmeshes());
			const AABB &aabb = submeshes[0].BoundingBox;
			submeshes[0].Transform = glm::rotate(glm::mat4(1.f), glm::radians(-90.f), glm::vec3(0.f, 0.f, 1.f));

			C3dUIComponent &uiComp = XAxisEnt->getComponent<C3dUIComponent>();
			uiComp.Name = STR_AXIS_X;
			uiComp.Handle = meshAsset->getHandle();
			uiComp.SourceHandle = meshSrc->getHandle();
			uiComp.SubmeshIndex = 0;
			uiComp.Visible = true;
			uiComp.State.Blend = Blend::Disable();
			uiComp.State.Mode = RenderMode::MaterialPreview;
			uiComp.State.DepthTest = DepthTest::Always();

			auto &boxComp = XAxisEnt->getComponent<BoxColliderComponent>();
			boxComp.HalfSize = glm::vec3(Math::IsZero(aabb.lenX() * 0.5f) ? 1e-3 : aabb.lenX() * 0.5f,
										 Math::IsZero(aabb.lenY() * 0.5f) ? 1e-3 : aabb.lenY() * 0.5f,
										 Math::IsZero(aabb.lenZ() * 0.5f) ? 1e-3 : aabb.lenZ() * 0.5f);
			boxComp.Material = PhysicsSystem::Get()->getMaterial(PhysicsMatType::Normal);
		}

		//Y axis
		{
			Ref<MeshSource> meshSrc = MeshFactory::CreateCylinder(config.Radius, config.Height, config.Itr);
			Ref<Asset> meshAsset = AssetsManager::CreateRuntimeAssets<StaticMesh>(meshSrc);

			Ref<Cylinder> cylinder = RefCast<MeshSource, Cylinder>(meshSrc);
			cylinder->setColor(glm::vec3(0.f, 1.f, 0.f));

			std::vector<Submesh> &submeshes = const_cast<std::vector<Submesh> &>(meshSrc->getSubmeshes());
			const AABB &aabb = submeshes[0].BoundingBox;

			C3dUIComponent &uiComp = YAxisEnt->getComponent<C3dUIComponent>();
			uiComp.Name = STR_AXIS_Y;
			uiComp.Handle = meshAsset->getHandle();
			uiComp.SourceHandle = meshSrc->getHandle();
			uiComp.SubmeshIndex = 0;
			uiComp.Visible = true;
			uiComp.State.Blend = Blend::Disable();
			uiComp.State.Mode = RenderMode::MaterialPreview;
			uiComp.State.DepthTest = DepthTest::Always();

			auto &boxComp = YAxisEnt->getComponent<BoxColliderComponent>();
			boxComp.HalfSize = glm::vec3(Math::IsZero(aabb.lenX() * 0.5f) ? 1e-3 : aabb.lenX() * 0.5f,
										 Math::IsZero(aabb.lenY() * 0.5f) ? 1e-3 : aabb.lenY() * 0.5f,
										 Math::IsZero(aabb.lenZ() * 0.5f) ? 1e-3 : aabb.lenZ() * 0.5f);
			boxComp.Material = PhysicsSystem::Get()->getMaterial(PhysicsMatType::Normal);
		}

		//Z axis
		{
			Ref<MeshSource> meshSrc = MeshFactory::CreateCylinder(config.Radius, config.Height, config.Itr);
			Ref<Asset> meshAsset = AssetsManager::CreateRuntimeAssets<StaticMesh>(meshSrc);

			Ref<Cylinder> cylinder = RefCast<MeshSource, Cylinder>(meshSrc);
			cylinder->setColor(glm::vec3(0.f, 0.f, 1.f));

			std::vector<Submesh> &submeshes = const_cast<std::vector<Submesh> &>(meshSrc->getSubmeshes());
			const AABB &aabb = submeshes[0].BoundingBox;
			submeshes[0].Transform = glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));

			C3dUIComponent &uiComp = ZAxisEnt->getComponent<C3dUIComponent>();
			uiComp.Name = STR_AXIS_Z;
			uiComp.Handle = meshAsset->getHandle();
			uiComp.SourceHandle = meshSrc->getHandle();
			uiComp.SubmeshIndex = 0;
			uiComp.Visible = true;
			uiComp.State.Blend = Blend::Disable();
			uiComp.State.Mode = RenderMode::MaterialPreview;
			uiComp.State.DepthTest = DepthTest::Always();

			auto &boxComp = ZAxisEnt->getComponent<BoxColliderComponent>();
			boxComp.HalfSize = glm::vec3(Math::IsZero(aabb.lenX() * 0.5f) ? 1e-3 : aabb.lenX() * 0.5f,
										 Math::IsZero(aabb.lenY() * 0.5f) ? 1e-3 : aabb.lenY() * 0.5f,
										 Math::IsZero(aabb.lenZ() * 0.5f) ? 1e-3 : aabb.lenZ() * 0.5f);
			boxComp.Material = PhysicsSystem::Get()->getMaterial(PhysicsMatType::Normal);
		}
	}

	void UiCoordinate3D::buildImpl(const ArrowConfig &config)
	{
		//X axis
		{
			Ref<MeshSource> meshSrc = MeshFactory::CreateArrow3D(config.CylinderRadius, config.CylinderHeight,
																 config.ConeRadius, config.ConeHeight, config.Itr);
			Ref<Asset> meshAsset = AssetsManager::CreateRuntimeAssets<StaticMesh>(meshSrc);

			Ref<Arrow3D> arrow = RefCast<MeshSource, Arrow3D>(meshSrc);
			arrow->setColor(glm::vec3(1.f, 0.f, 0.f));

			std::vector<Submesh> &submeshes = const_cast<std::vector<Submesh> &>(meshSrc->getSubmeshes());
			const AABB &aabb = submeshes[0].BoundingBox;
			glm::mat4 transform = glm::translate(glm::mat4(1.f), glm::vec3(config.Offset, 0.f, 0.f));
			submeshes[0].Transform = glm::rotate(transform, glm::radians(-90.f), glm::vec3(0.f, 0.f, 1.f));

			C3dUIComponent &uiComp = XAxisEnt->getComponent<C3dUIComponent>();
			uiComp.Name = STR_AXIS_X;
			uiComp.Handle = meshAsset->getHandle();
			uiComp.SourceHandle = meshSrc->getHandle();
			uiComp.SubmeshIndex = 0;
			uiComp.Visible = true;
			uiComp.State.Blend = Blend::Disable();
			uiComp.State.Mode = RenderMode::MaterialPreview;
			uiComp.State.DepthTest = DepthTest::Always();

			auto &boxComp = XAxisEnt->getComponent<BoxColliderComponent>();
			boxComp.HalfSize = glm::vec3(Math::IsZero(aabb.lenX() * 0.5f) ? 1e-3 : aabb.lenX() * 0.5f * UI3D_TOUCH_EXT,
										 Math::IsZero(aabb.lenY() * 0.5f) ? 1e-3 : aabb.lenY() * 0.5f * UI3D_TOUCH_EXT,
										 Math::IsZero(aabb.lenZ() * 0.5f) ? 1e-3 : aabb.lenZ() * 0.5f * UI3D_TOUCH_EXT);
			boxComp.Material = PhysicsSystem::Get()->getMaterial(PhysicsMatType::Normal);
		}

		//Y axis
		{
			Ref<MeshSource> meshSrc = MeshFactory::CreateArrow3D(config.CylinderRadius, config.CylinderHeight,
																 config.ConeRadius, config.ConeHeight, config.Itr);
			Ref<Asset> meshAsset = AssetsManager::CreateRuntimeAssets<StaticMesh>(meshSrc);

			Ref<Arrow3D> arrow = RefCast<MeshSource, Arrow3D>(meshSrc);
			arrow->setColor(glm::vec3(0.f, 1.f, 0.f));

			std::vector<Submesh> &submeshes = const_cast<std::vector<Submesh> &>(meshSrc->getSubmeshes());
			const AABB &aabb = submeshes[0].BoundingBox;
			submeshes[0].Transform = glm::translate(glm::mat4(1.f), glm::vec3(0.f, config.Offset, 0.f));

			C3dUIComponent &uiComp = YAxisEnt->getComponent<C3dUIComponent>();
			uiComp.Name = STR_AXIS_Y;
			uiComp.Handle = meshAsset->getHandle();
			uiComp.SourceHandle = meshSrc->getHandle();
			uiComp.SubmeshIndex = 0;
			uiComp.Visible = true;
			uiComp.State.Blend = Blend::Disable();
			uiComp.State.Mode = RenderMode::MaterialPreview;
			uiComp.State.DepthTest = DepthTest::Always();

			auto &boxComp = YAxisEnt->getComponent<BoxColliderComponent>();
			boxComp.HalfSize = glm::vec3(Math::IsZero(aabb.lenX() * 0.5f) ? 1e-3 : aabb.lenX() * 0.5f * UI3D_TOUCH_EXT,
										 Math::IsZero(aabb.lenY() * 0.5f) ? 1e-3 : aabb.lenY() * 0.5f * UI3D_TOUCH_EXT,
										 Math::IsZero(aabb.lenZ() * 0.5f) ? 1e-3 : aabb.lenZ() * 0.5f * UI3D_TOUCH_EXT);
			boxComp.Material = PhysicsSystem::Get()->getMaterial(PhysicsMatType::Normal);
		}

		//Z axis
		{
			Ref<MeshSource> meshSrc = MeshFactory::CreateArrow3D(config.CylinderRadius, config.CylinderHeight,
																 config.ConeRadius, config.ConeHeight, config.Itr);
			Ref<Asset> meshAsset = AssetsManager::CreateRuntimeAssets<StaticMesh>(meshSrc);

			Ref<Arrow3D> arrow = RefCast<MeshSource, Arrow3D>(meshSrc);
			arrow->setColor(glm::vec3(0.f, 0.f, 1.f));

			std::vector<Submesh> &submeshes = const_cast<std::vector<Submesh> &>(meshSrc->getSubmeshes());
			const AABB &aabb = submeshes[0].BoundingBox;
			glm::mat4 transform = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, config.Offset));
			submeshes[0].Transform = glm::rotate(transform, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));

			C3dUIComponent &uiComp = ZAxisEnt->getComponent<C3dUIComponent>();
			uiComp.Name = STR_AXIS_Z;
			uiComp.Handle = meshAsset->getHandle();
			uiComp.SourceHandle = meshSrc->getHandle();
			uiComp.SubmeshIndex = 0;
			uiComp.Visible = true;
			uiComp.State.Blend = Blend::Disable();
			uiComp.State.Mode = RenderMode::MaterialPreview;
			uiComp.State.DepthTest = DepthTest::Always();

			auto &boxComp = ZAxisEnt->getComponent<BoxColliderComponent>();
			boxComp.HalfSize = glm::vec3(Math::IsZero(aabb.lenX() * 0.5f) ? 1e-3 : aabb.lenX() * 0.5f * UI3D_TOUCH_EXT,
										 Math::IsZero(aabb.lenY() * 0.5f) ? 1e-3 : aabb.lenY() * 0.5f * UI3D_TOUCH_EXT,
										 Math::IsZero(aabb.lenZ() * 0.5f) ? 1e-3 : aabb.lenZ() * 0.5f * UI3D_TOUCH_EXT);
			boxComp.Material = PhysicsSystem::Get()->getMaterial(PhysicsMatType::Normal);
		}
	}

	UiRotationCtl::UiRotationCtl()
	{
		XTorus = Registry::Get()->create<C3dUIComponent, TransformComponent, BoxColliderComponent>();
		YTorus = Registry::Get()->create<C3dUIComponent, TransformComponent, BoxColliderComponent>();
		ZTorus = Registry::Get()->create<C3dUIComponent, TransformComponent, BoxColliderComponent>();
		build();
	}

	UiRotationCtl::~UiRotationCtl()
	{
	}

	void UiRotationCtl::build()
	{	
		const float radius = 0.8f;
		const float ringWidth = 0.03f;
		const uint32_t itr = 36;
		const uint32_t ringItr = 16;
		const float alpha = 0.6f;

		// Torus around x axis
		{
			Ref<MeshSource> meshSrc = MeshFactory::CreateTorus(radius, ringWidth, glm::vec3(1.f, 0.f, 0.f), itr, ringItr);
			meshSrc->as<Geometry>()->setAlpha(alpha);
			Ref<Asset> meshAsset = AssetsManager::CreateRuntimeAssets<StaticMesh>(meshSrc);

			std::vector<Submesh> &submeshes = const_cast<std::vector<Submesh> &>(meshSrc->getSubmeshes());			
			submeshes[0].Transform = glm::rotate(glm::mat4(1.f), glm::radians(90.f), AXIS_Y);				

			C3dUIComponent &uiComp = XTorus->getComponent<C3dUIComponent>();
			uiComp.Name = STR_TORUS_X;
			uiComp.Handle = meshAsset->getHandle();
			uiComp.SourceHandle = meshSrc->getHandle();
			uiComp.SubmeshIndex = 0; 
			uiComp.Visible = true;
			uiComp.State.Blend = Blend::Common();
			uiComp.State.Mode = RenderMode::MaterialPreview;
			uiComp.State.DepthTest = DepthTest::Common();
			uiComp.State.Cull = CullFace::Common();

			const AABB &aabb = submeshes[0].BoundingBox;
			auto &boxComp = XTorus->getComponent<BoxColliderComponent>();
			boxComp.HalfSize = glm::vec3(Math::IsZero(aabb.lenX() * 0.5f) ? 1e-3 : aabb.lenX() * 0.5f,
										 Math::IsZero(aabb.lenY() * 0.5f) ? 1e-3 : aabb.lenY() * 0.5f,
										 Math::IsZero(aabb.lenZ() * 0.5f) ? 1e-3 : aabb.lenZ() * 0.5f);
			boxComp.Material = PhysicsSystem::Get()->getMaterial(PhysicsMatType::Normal);
		}

		// Torus around y axis
		{
			Ref<MeshSource> meshSrc = MeshFactory::CreateTorus(radius, ringWidth, glm::vec3(0.f, 1.f, 0.f), itr, ringItr);
			meshSrc->as<Geometry>()->setAlpha(alpha);
			Ref<Asset> meshAsset = AssetsManager::CreateRuntimeAssets<StaticMesh>(meshSrc);			

			std::vector<Submesh> &submeshes = const_cast<std::vector<Submesh> &>(meshSrc->getSubmeshes());
			submeshes[0].Transform = glm::rotate(glm::mat4(1.f), glm::radians(-90.f), AXIS_X);

			C3dUIComponent &uiComp = YTorus->getComponent<C3dUIComponent>();
			uiComp.Name = STR_TORUS_Y;
			uiComp.Handle = meshAsset->getHandle();
			uiComp.SourceHandle = meshSrc->getHandle();
			uiComp.SubmeshIndex = 0;
			uiComp.Visible = true;
			uiComp.State.Blend = Blend::Common();
			uiComp.State.Mode = RenderMode::MaterialPreview;
			uiComp.State.DepthTest = DepthTest::Common();
			uiComp.State.Cull = CullFace::Common();

			const AABB &aabb = submeshes[0].BoundingBox;
			auto &boxComp = YTorus->getComponent<BoxColliderComponent>();
			boxComp.HalfSize = glm::vec3(Math::IsZero(aabb.lenX() * 0.5f) ? 1e-3 : aabb.lenX() * 0.5f,
										 Math::IsZero(aabb.lenY() * 0.5f) ? 1e-3 : aabb.lenY() * 0.5f,
										 Math::IsZero(aabb.lenZ() * 0.5f) ? 1e-3 : aabb.lenZ() * 0.5f);
			boxComp.Material = PhysicsSystem::Get()->getMaterial(PhysicsMatType::Normal);
		}

		// Torus around z axis
		{
			Ref<MeshSource> meshSrc = MeshFactory::CreateTorus(radius, ringWidth, glm::vec3(0.f, 0.f, 1.f), itr, ringItr);
			meshSrc->as<Geometry>()->setAlpha(alpha);
			Ref<Asset> meshAsset = AssetsManager::CreateRuntimeAssets<StaticMesh>(meshSrc);

			C3dUIComponent &uiComp = ZTorus->getComponent<C3dUIComponent>();
			uiComp.Name = STR_TORUS_Z;
			uiComp.Handle = meshAsset->getHandle();
			uiComp.SourceHandle = meshSrc->getHandle();
			uiComp.SubmeshIndex = 0;
			uiComp.Visible = true;
			uiComp.State.Blend = Blend::Common();
			uiComp.State.Mode = RenderMode::MaterialPreview;
			uiComp.State.DepthTest = DepthTest::Common();
			uiComp.State.Cull = CullFace::Common();

			std::vector<Submesh> &submeshes = const_cast<std::vector<Submesh> &>(meshSrc->getSubmeshes());
			const AABB &aabb = submeshes[0].BoundingBox;
			auto &boxComp = ZTorus->getComponent<BoxColliderComponent>();
			boxComp.HalfSize = glm::vec3(Math::IsZero(aabb.lenX() * 0.5f) ? 1e-3 : aabb.lenX() * 0.5f,
										 Math::IsZero(aabb.lenY() * 0.5f) ? 1e-3 : aabb.lenY() * 0.5f,
										 Math::IsZero(aabb.lenZ() * 0.5f) ? 1e-3 : aabb.lenZ() * 0.5f);
			boxComp.Material = PhysicsSystem::Get()->getMaterial(PhysicsMatType::Normal);
		}
	}
}