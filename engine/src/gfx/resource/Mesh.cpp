#include "Mesh.h"

#include "scene/Components.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Mesh"

namespace pio
{
	template<>
	bool Asset::is<Mesh>() const { return assetType() == AssetType::Mesh; }
}