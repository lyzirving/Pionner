#ifndef __PIONNER_SCENE_3D_MESH_H__
#define __PIONNER_SCENE_3D_MESH_H__

#include "asset/Asset.h"
#include "TriangleMesh.h"

namespace pio
{
	class Mesh : public Asset
	{
		OVERRIDE_ASSET_TYPE(AssetType::Mesh)
	public:
		Mesh() : Asset() {}
		virtual ~Mesh() = default;

	protected:
		TriangleMesh m_triMesh;

	private:
		friend class Factory;
	};

	template<>
	bool Asset::is<Mesh>() const;
}

#endif