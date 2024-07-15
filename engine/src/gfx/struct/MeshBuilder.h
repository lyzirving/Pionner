#ifndef __PIONNER_GFX_STRUCT_MESH_BUILDER_H__
#define __PIONNER_GFX_STRUCT_MESH_BUILDER_H__

#include "Geometry.h"
#include "MeshFactory.h"

#include "asset/AssetsManager.h"
#include "asset/AssimpMeshImporter.h"

#include "animation/Animator.h"
#include "animation/Animation.h"

#include "physics/PhysicsScene.h"
#include "physics/PhysicsSystem.h"
#include "physics/PhysicsActor.h"

namespace pio
{
	struct MeshBuildParam
	{
		Ref<MeshSource> meshSrc;
		Ref<PhysicsScene> physicWorld;
		RigidBodyComponent::Type RigidType{ RigidBodyComponent::Type::None };
		RenderState State{};
		Ref<Entity> Parent;
	};

	template <typename MeshT, typename MeshCompT, typename ... Comps>
	Ref<Asset> BuildMesh(const MeshBuildParam &param)
	{
		Ref<MeshSource> meshSrc = param.meshSrc;
		Ref<PhysicsScene> physicsScene = param.physicWorld;

		Ref<Entity> hostEnt = Registry::Get()->create<RelationshipComponent, MeshSourceComponent>(EntityClass::MeshSource, meshSrc->getName());
		hostEnt->getComponent<MeshSourceComponent>().SourceHandle = meshSrc->getHandle();

		PIO_RELATION_SET_SELF(hostEnt);
		if (param.Parent) 
		{	
			PIO_RELATION_SET_PARENT(hostEnt, param.Parent);
			PIO_RELATION_SET_CHILD(param.Parent, hostEnt);
		}

		Ref<MeshT> result = AssetsManager::CreateRuntimeAssets<MeshT>(meshSrc);

		// Animation
		AssetHandle animHandle{ NullAsset };
		if (meshSrc->hasAnimation())
		{
			AnimationComponent &comp = hostEnt->addComponent<AnimationComponent>();
			Ref<AnimationAsset> animAssets = AssetsManager::CreateRuntimeAssets<AnimationAsset>(meshSrc);
			animHandle = comp.Handle = animAssets->getHandle();
			comp.Selection = 0;
			comp.GPUSkinning = true;
		}

		// Submesh
		auto &submeshes = const_cast<std::vector<Submesh> &>(meshSrc->getSubmeshes());
		for (uint32_t i = 0; i < submeshes.size(); i++)
		{
			Ref<Entity> ent = Registry::Get()->create<RelationshipComponent, TransformComponent, MeshCompT, Comps...>(EntityClass::Mesh, submeshes[i].MeshName);
			submeshes[i].Ent = ent;

			PIO_RELATION_SET_SELF(ent);
			PIO_RELATION_SET_CHILD(hostEnt, ent);
			PIO_RELATION_SET_PARENT(ent, hostEnt);			

			if (ent->hasComponent<MeshComponent>())
			{
				MeshComponent &comp = ent->getComponent<MeshComponent>();
				comp.Handle = result->getHandle();
				comp.SourceHandle = meshSrc->getHandle();
				comp.SubmeshIndex = i;
				comp.Visible = true;
				comp.State = param.State;
			}
			else if (ent->hasComponent<StaticMeshComponent>())
			{
				StaticMeshComponent &comp = ent->getComponent<StaticMeshComponent>();
				comp.Handle = result->getHandle();
				comp.SourceHandle = meshSrc->getHandle();
				comp.SubmeshIndex = i;
				comp.Visible = true;
				comp.State = param.State;
			}

			if (ent->hasComponent<BoxColliderComponent>())
			{
				AABB aabb(submeshes[i].Transform * glm::vec4(submeshes[i].BoundingBox.Min, 1.f),
						  submeshes[i].Transform * glm::vec4(submeshes[i].BoundingBox.Max, 1.f));				
				BoxColliderComponent &comp = ent->getComponent<BoxColliderComponent>();
				//comp.HalfSize = 0.5f * glm::vec3(aabb.lenX(), aabb.lenY(), aabb.lenZ());
				comp.HalfSize = glm::vec3(Math::IsZero(aabb.lenX() * 0.5f) ? 1e-3 : aabb.lenX() * 0.5f,
										  Math::IsZero(aabb.lenY() * 0.5f) ? 1e-3 : aabb.lenY() * 0.5f,
										  Math::IsZero(aabb.lenZ() * 0.5f) ? 1e-3 : aabb.lenZ() * 0.5f);
				comp.Material = PhysicsSystem::Get()->getMaterial(PhysicsMatType::Normal);
				physicsScene->createActor<MeshCompT>(ent, param.RigidType);
			}
		}

		return result;
	}

	template <typename ... Comps>
	Ref<Asset> CreateDynamicMesh(const MeshBuildParam &param)
	{
		return BuildMesh<Mesh, MeshComponent, Comps...>(param);
	}

	template <typename ... Comps>
	Ref<Asset> CreateStaticMesh(const MeshBuildParam &param)
	{
		return BuildMesh<StaticMesh, StaticMeshComponent, Comps...>(param);
	}
}

#endif