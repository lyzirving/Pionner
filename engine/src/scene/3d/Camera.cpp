#include "Camera.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Camera"

namespace pio
{
	void Camera::makeCulling()
	{
		if (m_drawEnts.empty())
			return;

		//[TODO]
	}

	template<>
	bool Asset::is<Camera>() const { return type() == AssetType::Camera; }
}