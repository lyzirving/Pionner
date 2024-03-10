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
		RigidBodyComponent::Type RigidType{ RigidBodyComponent::Type::None };
		RenderState State{};
		Ref<Entity> Parent;
	};

	template <typename MeshT, typename MeshCompT, typename ... Comps>
	Ref<Asset> BuildMesh(Ref<MeshSource> &meshSrc, Ref<PhysicsScene> &physicsScene, const MeshBuildParam &param)
	{
		Ref<Entity> hostEnt = Registry::Get()->create<RelationshipComponent, MeshSourceComponent>(NodeType::MeshSource);
		hostEnt->getComponent<MeshSourceComponent>().SourceHandle = meshSrc->getHandle();

		PIO_RELATION_SET_TAG(hostEnt, meshSrc->getName());
		PIO_RELATION_SET_SELF_INDEX(hostEnt, hostEnt->getCacheIndex());
		if (param.Parent) 
		{	
			PIO_RELATION_SET_PARENT_INDEX(hostEnt, param.Parent->getCacheIndex());
			PIO_RELATION_SET_CHILD_INDEX(param.Parent, hostEnt->getCacheIndex());
		}

		Ref<Asset> result = AssetsManager::CreateRuntimeAssets<MeshT>(meshSrc);

		// Animation
		AssetHandle animHandle{ NullAsset };
		if (meshSrc->hasAnimation())
		{
			AnimationComponent &comp = hostEnt->addComponent<AnimationComponent>();
			Ref<Asset> animAssets = AssetsManager::CreateRuntimeAssets<AnimationAsset>(meshSrc);
			animHandle = comp.Handle = animAssets->getHandle();
			comp.Selection = 0;
			comp.GPUSkinning = true;
		}

		// Submesh
		auto &submeshes = const_cast<std::vector<Submesh> &>(meshSrc->getSubmeshes());
		for (uint32_t i = 0; i < submeshes.size(); i++)
		{
			Ref<Entity> ent = Registry::Get()->create<RelationshipComponent, TransformComponent, MeshCompT, Comps...>(NodeType::Mesh);	
			submeshes[i].Ent = ent;

			PIO_RELATION_SET_TAG(ent, submeshes[i].MeshName);
			PIO_RELATION_SET_SELF_INDEX(ent, ent->getCacheIndex());
			PIO_RELATION_SET_CHILD_INDEX(hostEnt, ent->getCacheIndex());
			PIO_RELATION_SET_PARENT_INDEX(ent, hostEnt->getCacheIndex());

			const AABB &aabb = submeshes[i].BoundingBox;			

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
				BoxColliderComponent &comp = ent->getComponent<BoxColliderComponent>();
				comp.HalfSize = submeshes[i].Transform * glm::vec4(glm::vec3(aabb.lenX() * 0.5f, aabb.lenY() * 0.5f, aabb.lenZ() * 0.5f), 1.f);
				comp.Material = PhysicsSystem::Get()->getMaterial(PhysicsMatType::Normal);
				physicsScene->createActor<MeshCompT>(ent, param.RigidType);
			}
		}

		return result;
	}

	template <typename ... Comps>
	Ref<Asset> CreateMesh(Ref<MeshSource> &meshSrc, Ref<PhysicsScene> &world, const MeshBuildParam &param)
	{
		return BuildMesh<Mesh, MeshComponent, Comps...>(meshSrc, world, param);
	}

	template <typename ... Comps>
	Ref<Asset> CreateStaticMesh(Ref<MeshSource> &meshSrc, Ref<PhysicsScene> &world, const MeshBuildParam &param)
	{
		return BuildMesh<StaticMesh, StaticMeshComponent, Comps...>(meshSrc, world, param);
	}
}

#endif