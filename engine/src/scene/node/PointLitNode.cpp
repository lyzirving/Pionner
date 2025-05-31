#include "PointLitNode.h"
#include "PointLitGroup.h"
#include "SpriteNode.h"
#include "WireframeNode.h"

#include "gfx/renderer/RenderContext.h"

#include "scene/Scene.h"

#include "component/PointLitComponent.h"
#include "component/TransformComponent.h"

#include "asset/AssetMgr.h"
#include "asset/mesh/StaticMesh.h"
#include "asset/mesh/MeshFactory.h"
#include "asset/image/Image.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "PointLitNode"

namespace pio
{
	void PointLitNode::OnSelect()
	{
		m_Gizmo->SetVisible(true);
	}

	void PointLitNode::OnDeselect()
	{
		m_Gizmo->SetVisible(false);
	}

	void PointLitNode::SetColor(const glm::vec3& color)
	{
		GetComponent<PointLitComponent>()->SetColor(color);
	}

	void PointLitNode::SetIntensity(float intensity)
	{
		GetComponent<PointLitComponent>()->SetIntensity(std::max(intensity, 0.f));
	}

	void PointLitNode::SetRadius(float radius)
	{
		auto comp = GetComponent<PointLitComponent>();
		if(comp->SetRadius(std::max(radius, 0.f)))
		{
			auto subMesh = MeshFactory::MakePointLightGizmo(comp->GetRadius());
			subMesh.MaterialIdx = 0;
			m_Gizmo->SetSubMesh(0, subMesh);
		}
	}

	void PointLitNode::SetFalloff(float falloff)
	{
		GetComponent<PointLitComponent>()->SetFalloff(falloff);
	}

	void PointLitNode::SetShadowMode(ShadowMode mode)
	{
		GetComponent<PointLitComponent>()->SetMode(mode);
	}

	void PointLitNode::SetShadowIntensity(float intensity)
	{
		GetComponent<PointLitComponent>()->SetShadowIntensity(intensity);
	}

	void PointLitNode::SetShadowBias(float bias)
	{
		GetComponent<PointLitComponent>()->SetBias(bias);
	}

	int32_t PointLitNode::GetIndex() const
	{
		return GetComponent<PointLitComponent>()->GetIndex();
	}

	const glm::vec3& PointLitNode::GetColor() const
	{
		return GetComponent<PointLitComponent>()->GetColor();
	}

	float PointLitNode::GetIntensity() const
	{
		return GetComponent<PointLitComponent>()->GetIntensity();
	}

	float PointLitNode::GetRadius() const
	{
		return GetComponent<PointLitComponent>()->GetRadius();
	}

	float PointLitNode::GetFalloff() const
	{
		return GetComponent<PointLitComponent>()->GetFalloff();
	}

	bool PointLitNode::IsCastShadow() const
	{
		return GetComponent<PointLitComponent>()->GetbCastShadow();
	}

	ShadowMode PointLitNode::GetShadowMode() const
	{
		return GetComponent<PointLitComponent>()->GetMode();
	}

	float PointLitNode::GetShadowIntensity() const
	{
		return GetComponent<PointLitComponent>()->GetShadowIntensity();
	}

	float PointLitNode::GetShadowBias() const
	{
		return GetComponent<PointLitComponent>()->GetBias();
	}

	void PointLitNode::OnInit()
	{
		AddComponent<PointLitComponent>();

		m_Sprite = m_Scene.lock()->CreateChildNode<SpriteNode>(Self<Node>(), "PointLitSprite");
		auto image = AssetMgr::Get()->GetAsset<Image>(Path::ImageKey(ImageAsset::ICON_POINT_LIT));
		m_Sprite->SetImage(image);
		m_Sprite->SetFlipY(true);
		m_Sprite->SetShowOutliner(false);
		m_Sprite->SetMobility(Mobility_Ignore);
		m_Sprite->SetVisible(true);

		m_Gizmo = m_Scene.lock()->CreateChildNode<WireframeNode>(Self<Node>(), "PointLitGizmo");
		auto gizmoMesh = AssetMgr::Get()->GetAsset<StaticMesh>(Path::MeshKey(MeshAsset::POINTLIT_GIZMO));
		m_Gizmo->SetMesh(gizmoMesh);
		m_Gizmo->SetColor(glm::vec3(0.964f, 0.953f, 0.051f));
		m_Sprite->SetMobility(Mobility_Ignore);
		m_Gizmo->SetVisible(false);
	}

	void PointLitNode::OnTickComponents(const Ref<RenderContext>& context)
	{
		auto litComp = GetComponent<PointLitComponent>();
		litComp->SetPosition(GetComponent<TransformComponent>()->GetPosition());
	}

	void PointLitNode::OnGroupIndexChange()
	{
		auto comp = GetComponent<PointLitComponent>();
		comp->SetIndex(m_GroupIndex);
		comp->ApplyAttrsChange();
	}
}