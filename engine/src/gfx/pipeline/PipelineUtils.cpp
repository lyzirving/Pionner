#include "PipelineUtils.h"

#include "asset/AssetMgr.h"

#include "scene/Entity.h"
#include "scene/Components.h"
#include "scene/3d/Camera.h"
#include "scene/resources/Mesh.h"
#include "scene/resources/Material.h"

#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "PipelineUtils"

namespace pio
{
	namespace pipeline
	{
		std::vector<Ref<Camera>> FetchCamera(Ref<RenderContext>& context, const std::list<Ref<Entity>>& entities)
		{
			std::vector<Ref<Camera>> cameras;
			for (auto& ent : entities)
			{
				CameraComponent* camComp{ nullptr };
				TransformComponent* transComp{ nullptr };
				if (!(camComp = ent->getComponent<CameraComponent>())||
					!(transComp = ent->getComponent<TransformComponent>()))
				{
					return cameras;
				}
				auto cam = AssetMgr::GetRuntimeAsset<Camera>(camComp->Uid);

				cam->setPrjType(camComp->PrjType);
				cam->setFov(camComp->Fov);
				cam->setSize(camComp->Size);
				cam->setPosition(transComp->Position);

				cameras.push_back(cam);
			}
			return cameras;
		}

		void ProcessMeshEnt(Ref<RenderContext>& context, Ref<Entity>& entity, /*out*/RenderingData& renderingData)
		{
			if (!entity)
				return;

			MeshFilter* filter{ nullptr };
			MeshRenderer* render{ nullptr };
			TransformComponent* transComp{ nullptr };
			if (!(filter = entity->getComponent<MeshFilter>()) ||
				!(render = entity->getComponent<MeshRenderer>()) ||
				!(transComp = entity->getComponent<TransformComponent>()) ||
				!filter->Enable || !render->Enable)
			{
				return;
			}

			Ref<Mesh> mesh = AssetMgr::GetRuntimeAsset<Mesh>(filter->Uid);
			Ref<Material> mat = AssetMgr::GetRuntimeAsset<Material>(render->MatUid);

			mesh->setUp(context);
			mesh->update(*transComp);

			MeshCmd cmd;
			cmd.Data = mesh->data();
			cmd.Uniforms = mesh->unims();
			cmd.Material = mat;

			switch (mat->renderingMode())
			{
			case RenderingMode_Opaque:
			{
				renderingData.OpaqueMeshCmd.push_back(std::move(cmd));
				break;
			}
			case RenderingMode_Transparent:
			{
				renderingData.TransparentMeshCmd.push_back(std::move(cmd));
				break;
			}
			default:
				break;
			}
		}
	}
}