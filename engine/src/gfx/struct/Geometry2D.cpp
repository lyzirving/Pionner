#include "Geometry2D.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Geometry2D"

namespace pio
{
	void LineMesh::clear()
	{
		Vertex.clear();
		Indices.clear();
		Capacity = 0;
	}

	void QuadMesh::clear()
	{
		Vertex.clear();
		Indices.clear();
		Capacity = 0;
	}

	template<>
	bool Asset::is<LineMesh>() const { return this->getAssetType() == AssetType::LineMesh; }

	template<>
	bool Asset::is<QuadMesh>() const { return this->getAssetType() == AssetType::QuadMesh; }
}