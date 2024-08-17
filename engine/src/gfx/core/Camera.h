#ifndef __PIONNER_GFX_CORE_CAMERA_H__
#define __PIONNER_GFX_CORE_CAMERA_H__

#include "asset/Asset.h"

namespace pio
{
	class Camera : public Asset
	{
		OVERRIDE_ASSET_TYPE(AssetType::Camera)
	public:

	public:
		Camera() {}
		~Camera() = default;
	};

	template<>
	bool Asset::is<Camera>() const;
}

#endif