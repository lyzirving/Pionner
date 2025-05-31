#include "SkinnedMesh.h"

#include "Skeleton.h"
#include "AnimationClip.h"

#include "asset/material/Material.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "SkinnedMesh"

namespace pio
{
	SkinnedMesh::SkinnedMesh(const ImportParams& params) : StaticMesh(params)
	{
	}

	Ref<Asset> SkinnedMesh::SpawnChild()
	{
		auto mesh = CreateRef<SkinnedMesh>(m_Params);
		mesh->m_SubMeshes = m_SubMeshes;
		mesh->m_Root = m_Root;
		mesh->m_Materials = m_Materials;
		mesh->m_Skeleton = m_Skeleton;
		mesh->m_AnimationClips = m_AnimationClips;
		mesh->m_Owner = Self<Asset>();
		return mesh;
	}
}