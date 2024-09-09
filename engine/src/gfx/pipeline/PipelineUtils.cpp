#include "PipelineUtils.h"

#include "asset/AssetMgr.h"

#include "scene/Entity.h"
#include "scene/Components.h"
#include "scene/3d/Camera.h"
#include "scene/resources/Mesh.h"
#include "scene/resources/Material.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/resource/MeshRenderBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "PipelineUtils"

namespace pio
{
	namespace Pipeline
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
				cam->setAspect(camComp->Aspect);
				cam->setPosition(transComp->Position);
				cam->setEuler(transComp->Rotation);

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

			auto mesh = AssetMgr::GetRuntimeAsset<Mesh>(filter->Uid);
			mesh->update(context, *transComp);

			Ref<Material> material = AssetMgr::GetRuntimeAsset<Material>(render->MatUid);
			Ref<MeshRenderBuffer> buff = AssetMgr::GetRuntimeAsset<MeshRenderBuffer>(render->BuffUid);
			material->update(context);
			buff->update(context, mesh);

			MeshRenderingItem item;
			item.RenderBuffFilter = render->BuffUid;
			item.MaterialFilter = render->MatUid;

			switch(material->renderingMode())
			{
				case RenderingMode_Opaque:
				{
					renderingData.OpaqueMeshItems.push_back(std::move(item));
					break;
				}
				case RenderingMode_Transparent:
				{
					renderingData.TransparentMeshItems.push_back(std::move(item));
					break;
				}
				default:
					break;
			}
		}
	}
}