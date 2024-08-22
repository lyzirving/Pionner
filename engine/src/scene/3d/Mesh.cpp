#include "Mesh.h"

namespace pio
{
	template<>
	bool Asset::is<Mesh>() const { return assetType() == AssetType::Mesh; }
}