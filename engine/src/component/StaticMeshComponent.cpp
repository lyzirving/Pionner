#include "StaticMeshComponent.h"
#include "TransformComponent.h"

#include "asset/mesh/StaticMesh.h"
#include "asset/material/Material.h"

#include "gfx/renderer/MeshRenderBuffer.h"
#include "gfx/renderer/UMotionBuffer.h"
#include "gfx/renderer/RenderContext.h"

#include "scene/node/StaticMeshNode.h"
#include "scene/node/WireframeNode.h"
#include "scene/node/SpriteNode.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "StaticMeshComponent"

namespace pio
{
	void StaticMeshComponent::OnMeshChange()
	{
		if(!m_Mesh)
			return;

		m_Mesh->OnInit();
		CreateBuffer();
		m_MaterialElems.assign(m_Mesh->m_Materials.begin(), m_Mesh->m_Materials.end());
	}

	void StaticMeshComponent::OnbCastShadowChange()
	{
	}

	void StaticMeshComponent::SetSubMesh(int32_t index, const SubMesh& mesh)
	{
		if(!m_Mesh)
			return;

		auto& subMeshList = m_Mesh->GetSubMesh();
		if(index >= subMeshList.size())
		{
			LOGE("err! index[%d] out of bounds[%u]", index, subMeshList.size());
			return;
		}
		subMeshList[index] = mesh;
		UpdateIndexedBuffer(index);
	}

	void StaticMeshComponent::SetTransformComponent(const Ref<TransformComponent>& comp)
	{
		m_TransComp = comp;
	}

	void StaticMeshComponent::OnTransformChange()
	{
		if(!m_TransComp)
			return;

		if(!m_Mesh)
			return;

		auto* node = m_Host.lock()->As<StaticMeshNode>();
		const auto& subMesh = m_Mesh->GetSubMesh();
		bool bSkeletalOn = false;
		for(size_t i = 0; i < subMesh.size(); i++)
		{
			const auto& mesh = subMesh[i];
			auto* buffer = m_MotionBuffers[i]->As<UMotionBuffer>();
			buffer->SetTransform(m_TransComp->GetMat());
			buffer->SetLocalTransform(mesh.Transform);
			buffer->SetSkeletalOn(false);

			if(node->GetMobility() != Mobility_Ignore)
			{
				node->m_BoundingBox.Union((m_TransComp->GetMat() * mesh.Transform) * mesh.BoundingBox);
			}
		}
	}

	void StaticMeshComponent::OnTick()
	{
		OnTransformChange();

		for(size_t i = 0; i < m_MotionBuffers.size(); ++i)
		{
			m_MotionBuffers[i]->As<UMotionBuffer>()->Upload();
		}

		for(auto& elem : m_MaterialElems)
		{
			elem->OnAttrsChange();
		}
	}

	void StaticMeshComponent::OnRender(RenderingData& data)
	{
		if(!m_Mesh)
			return;

		bool bWireframe = m_Host.lock()->Is<WireframeNode>();
		bool bSprite = m_Host.lock()->Is<SpriteNode>();

		const auto& subMesh = m_Mesh->GetSubMesh();
		const auto& materialList = m_Mesh->GetMaterial();
		for(size_t i = 0; i < subMesh.size(); i++)
		{
			const auto& mesh = subMesh[i];
			const auto& material = materialList[mesh.MaterialIdx];

			MeshRenderingItem item;
			item.Mode = material->IsTranslucency() ? RenderingMode_Transparent : RenderingMode_Opaque;
			item.DrawFlag = bWireframe ? DrawMode_Line : DrawMode_Triangle;
			item.MeshBuff = m_RenderBuff[i];
			item.Material = material;
			item.UniformBuffers.push_back(m_MotionBuffers[i]);

			if(bWireframe)
			{
				data.WireframeItems.push_back(std::move(item));
			}
			else if(bSprite)
			{
				data.SpriteItems.push_back(std::move(item));
			}
			else
			{
				if(material->IsTranslucency())
				{
					if(m_TransComp)
					{
						auto aabb = (m_TransComp->GetMat() * mesh.Transform) * mesh.BoundingBox;
						item.Center = aabb.Center();
					}
					data.TransparentMeshItems.push_back(item);
				}
				else
					data.OpaqueMeshItems.push_back(item);

				if(material->IsCastShadow())
					data.ShadowCasterItems.push_back(item);

				if(material->AnyFlag())
					data.PermutationFlags |= material->GetPermutationFlags();
			}
		}
	}

	void StaticMeshComponent::CreateBuffer()
	{
		if(!m_Mesh)
			return;

		auto context = m_Context.lock();
		if(!context)
		{
			LOGE("err! context is invalid");
			return;
		}

		const auto& subMesh = m_Mesh->GetSubMesh();
		m_RenderBuff.clear();
		m_MotionBuffers.clear();

		m_RenderBuff.reserve(subMesh.size());
		m_MotionBuffers.reserve(subMesh.size());
		for(size_t i = 0; i < subMesh.size(); i++)
		{
			const auto& mesh = subMesh[i];
			m_RenderBuff.emplace_back(CreateRef<MeshRenderBuffer>());
			m_RenderBuff[i]->Setup(context, mesh.Vertice, mesh.Indice);

			std::string name(mesh.Name);
			name.append("_UMotionBuffer");
			m_MotionBuffers.emplace_back(CreateRef<UMotionBuffer>(context, name));
		}
	}

	void StaticMeshComponent::UpdateIndexedBuffer(int32_t index)
	{
		if(!m_Mesh)
			return;

		auto context = m_Context.lock();
		if(!context)
		{
			LOGE("err! context is invalid");
			return;
		}

		const auto& subMesh = m_Mesh->GetSubMesh();
		if(index >= subMesh.size())
		{
			LOGE("err! index[%d] out of bounds[%u]", subMesh.size());
			return;
		}
		if(m_RenderBuff.empty() || index >= m_RenderBuff.size())
		{
			CreateBuffer();
		}
		else
		{
			LOGD("update submesh[%d]'s buffer", index);
			const auto& mesh = subMesh[index];
			m_RenderBuff[index] = CreateRef<MeshRenderBuffer>();
			m_RenderBuff[index]->Setup(context, mesh.Vertice, mesh.Indice);
		}
	}
}