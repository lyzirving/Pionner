#ifndef __PIONNER_SCENE_3D_CAMERA_H__
#define __PIONNER_SCENE_3D_CAMERA_H__

#include "asset/Asset.h"

namespace pio
{
	class Camera : public Asset
	{
		OVERRIDE_ASSET_TYPE(AssetType::Camera)
	public:
		Camera() : Asset() {}
		~Camera() = default;
	};

	template<>
	bool Asset::is<Camera>() const;
}

#endif