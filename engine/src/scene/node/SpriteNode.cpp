#include "SpriteNode.h"
#include "CameraNode.h"

#include "gfx/renderer/RenderContext.h"

#include "component/StaticMeshComponent.h"
#include "component/TransformComponent.h"
#include "component/CameraComponent.h"

#include "asset/AssetMgr.h"
#include "asset/mesh/StaticMesh.h"
#include "asset/material/TexturedMaterial.h"
#include "asset/image/Image.h"

#include "scene/Scene.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "SpriteNode"

namespace pio
{	
	void SpriteNode::OnInit()
	{		
		StaticMeshNode::OnInit();
		auto comp = GetComponent<StaticMeshComponent>();

		auto mesh = RefCast<Asset, StaticMesh>(AssetMgr::Get()->GetAsset<StaticMesh>(Path::MeshKey(MeshAsset::PLANE))->SpawnChild());
		auto textureMaterial = AssetMgr::Get()->GetAsset<TexturedMaterial>(Path::MaterialKey(MatAsset::TEXTURED))->SpawnChild();

		mesh->GetMaterial()[0] = RefCast<Asset, TexturedMaterial>(textureMaterial);
		comp->SetMesh(mesh);
	}

	void SpriteNode::OnTickComponents(const Ref<RenderContext>& context)
	{	
		auto transComp = GetComponent<TransformComponent>();

		auto billboard = Rotator(Math::BillboardRotation(GetScene()->GetMainCamera()->GetComponent<CameraComponent>()->GetViewMat()));
		Rotator rotator(90.f, 0.f, 0.f);
		transComp->SetRotation(billboard * rotator);

		transComp->OnTick();
		GetComponent<StaticMeshComponent>()->OnTick();
	}

	void SpriteNode::SetFlipY(bool flip)
	{
		auto& material = GetComponent<StaticMeshComponent>()->GetMesh()->GetMaterial()[0];
		material->As<TexturedMaterial>()->SetFlipY(flip);
	}

	void SpriteNode::SetImage(const Ref<Image>& image)
	{
		if(!image)
			return;

		auto mesh = GetComponent<StaticMeshComponent>()->GetMesh();
		mesh->GetMaterial()[0]->As<TexturedMaterial>()->SetTexture(image);
	}
}