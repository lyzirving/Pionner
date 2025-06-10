#include "DirectionalLitNode.h"
#include "WireframeNode.h"

#include "GlobalSettings.h"

#include "gfx/rhi/UniformBuffer.h"

#include "gfx/renderer/ShadowMap.h"
#include "gfx/renderer/RenderContext.h"

#include "component/TransformComponent.h"
#include "component/DirectionalLitComponent.h"

#include "scene/Scene.h"

#include "asset/AssetMgr.h"
#include "asset/mesh/StaticMesh.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "DirectionalLitNode"

namespace pio
{
	void DirectionalLitNode::OnInit()
	{
		auto comp = AddComponent<DirectionalLitComponent>();
		comp->SetTransformComponent(GetComponent<TransformComponent>());

		m_Gizmo = m_Scene.lock()->CreateChildNode<WireframeNode>(Self<Node>(), "DirLitGizmo");
		auto gizmoMesh = AssetMgr::Get()->GetAsset<StaticMesh>(Path::MeshKey(MeshAsset::DIRLIT_GIZMO));
		m_Gizmo->SetMesh(gizmoMesh);
		m_Gizmo->SetScale(glm::vec3(1.f, 1.f, -1.f));
		m_Gizmo->SetColor(glm::vec3(0.964f, 0.953f, 0.051f));
		m_Gizmo->SetMobility(Mobility_Ignore);
		m_Gizmo->SetVisible(false);
	}

	void DirectionalLitNode::OnSelect()
	{
		if(m_Gizmo)
		{
			m_Gizmo->SetVisible(true);
		}
	}

	void DirectionalLitNode::OnDeselect()
	{
		if(m_Gizmo)
		{
			m_Gizmo->SetVisible(false);
		}
	}

	void DirectionalLitNode::SetColor(const glm::vec3& color)
	{
		GetComponent<DirectionalLitComponent>()->SetColor(color);
	}

	void DirectionalLitNode::SetIntensity(float intensity)
	{
		GetComponent<DirectionalLitComponent>()->SetIntensity(std::max(intensity, 0.f));
	}

	void DirectionalLitNode::SetShadowMode(ShadowMode mode)
	{
		GetComponent<DirectionalLitComponent>()->SetMode(mode);
	}

	void DirectionalLitNode::SetShadowIntensity(float intensity)
	{
		GetComponent<DirectionalLitComponent>()->SetShadowIntensity(intensity);
	}

	void DirectionalLitNode::SetShadowBias(float bias)
	{
		GetComponent<DirectionalLitComponent>()->SetBias(bias);
	}

	const glm::vec3& DirectionalLitNode::GetColor() const
	{
		return GetComponent<DirectionalLitComponent>()->GetColor();
	}

	float DirectionalLitNode::GetIntensity() const
	{
		return GetComponent<DirectionalLitComponent>()->GetIntensity();
	}

	bool DirectionalLitNode::IsCastShadow() const
	{
		return GetComponent<DirectionalLitComponent>()->GetbCastShadow();
	}

	ShadowMode DirectionalLitNode::GetShadowMode() const
	{
		return GetComponent<DirectionalLitComponent>()->GetMode();
	}

	float DirectionalLitNode::GetShadowIntensity() const
	{
		return GetComponent<DirectionalLitComponent>()->GetShadowIntensity();
	}

	float DirectionalLitNode::GetShadowBias() const
	{
		return GetComponent<DirectionalLitComponent>()->GetBias();
	}

	void DirectionalLitNode::OnTickComponents(const Ref<RenderContext>& context)
	{
		GetComponent<TransformComponent>()->OnTick();
		GetComponent<DirectionalLitComponent>()->OnTick();
	}

	void DirectionalLitNode::OnRender(const Ref<RenderContext>& context)
	{
		auto& data = context->GetRenderingData();
		auto comp = GetComponent<DirectionalLitComponent>();

		auto buffer = comp->GetUBuffer();
		data.UBuffs[buffer->Binding()] = buffer;

		data.Pipeline.DirLitShadowMap = comp->GetShadowMap();
	}
}