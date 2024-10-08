#include "Geometry2D.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Geometry2D"

namespace pio
{
	void LineSegment::clear()
	{
		Vertex.clear();
		Indices.clear();
		Geometry2D::clear();
	}

	void QuadMesh::clear()
	{
		Vertex.clear();
		Indices.clear();
		Geometry2D::clear();
	}

	template<>
	bool Asset::is<LineSegment>() const { return this->getAssetType() == AssetType::LineSegment; }

	template<>
	bool Asset::is<QuadMesh>() const { return this->getAssetType() == AssetType::QuadMesh; }	
}