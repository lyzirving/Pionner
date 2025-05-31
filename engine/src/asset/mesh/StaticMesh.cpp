#include "StaticMesh.h"

#include "asset/material/Material.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "StaticMesh"

namespace pio
{
	StaticMesh::StaticMesh(const ImportParams& params) : Asset(params)
	{
	}

	void StaticMesh::OnInit()
	{
		if(m_SubMeshes.empty())
		{
			LOGE("StaticMesh[%s]'s submesh is empty", m_Params.Name.c_str());
			return;
		}
		
		for(size_t i = 0; i < m_SubMeshes.size(); i++)
		{
			auto& subMesh = m_SubMeshes[i];
			m_Materials[subMesh.MaterialIdx]->Init();
		}
	}

	Ref<Asset> StaticMesh::SpawnChild()
	{
		auto mesh = CreateRef<StaticMesh>(m_Params);
		mesh->m_SubMeshes = m_SubMeshes;
		mesh->m_Root = m_Root;
		mesh->m_Materials = m_Materials;
		mesh->m_Owner = Self<Asset>();
		return mesh;
	}
}