#include "PlaneNode.h"

#include "asset/AssetMgr.h"
#include "asset/mesh/StaticMesh.h"
#include "asset/mesh/MeshFactory.h"

#include "component/TransformComponent.h"
#include "component/StaticMeshComponent.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "PlaneNode"

namespace pio
{
	void PlaneNode::OnInit()
	{
		StaticMeshNode::OnInit();
		auto mesh = AssetMgr::Get()->GetAsset(Path::MeshKey(MeshAsset::PLANE))->SpawnChild();
		GetComponent<StaticMeshComponent>()->SetMesh(RefCast<Asset, StaticMesh>(mesh));
	}
}