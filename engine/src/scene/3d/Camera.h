#ifndef __PIONNER_SCENE_3D_CAMERA_H__
#define __PIONNER_SCENE_3D_CAMERA_H__

#include "asset/Asset.h"
#include "gfx/data/CameraData.h"

namespace pio
{
	struct PendingData;

	class Camera : public Asset
	{
		OVERRIDE_ASSET_TYPE(AssetType::Camera)
	public:
		Camera() : Asset() {}
		~Camera() = default;

		void culling(PendingData& pendingData);
		void setDepth(int32_t depth) { m_depth = depth; }

		int32_t depth() const { return m_depth; }

	private:
		int32_t m_depth{ 0 };
		CameraData m_data{};
	};

	template<>
	bool Asset::is<Camera>() const;
}

#endif