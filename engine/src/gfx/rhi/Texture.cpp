#include "Texture.h"

namespace pio
{
	template<>
	bool Asset::is<Texture>() const { return assetType() == AssetType::Texture; }
}