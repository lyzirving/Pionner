#include "PipelineUtils.h"

#include "asset/AssetMgr.h"

#include "scene/Entity.h"
#include "scene/Components.h"
#include "scene/3d/Camera.h"

#include "gfx/resource/Mesh.h"
#include "gfx/resource/Material.h"
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
		void UpdateCamera(Ref<RenderContext>& context, Ref<Entity>& entity, Ref<Camera>& camera)
		{
			auto* camComp = entity->getComponent<CameraComponent>();
			auto* tranComp = entity->getComponent<TransformComponent>();

			camera->setPrjType(camComp->PrjType);
			camera->setFov(camComp->Fov);
			camera->setSize(camComp->Size);
			camera->setAspect(camComp->Aspect);
			camera->setPosition(tranComp->Position);
			camera->setEuler(tranComp->Rotation);			

			camera->update(context);
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