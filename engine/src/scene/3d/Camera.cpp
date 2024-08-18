#include "Camera.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Camera"

namespace pio
{
	template<>
	bool Asset::is<Camera>() const { return type() == AssetType::Camera; }
}