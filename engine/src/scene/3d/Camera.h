#ifndef __PIONNER_SCENE_3D_CAMERA_H__
#define __PIONNER_SCENE_3D_CAMERA_H__

#include "asset/Asset.h"

namespace pio
{
	class Entity;

	class Camera : public Asset
	{
		OVERRIDE_ASSET_TYPE(AssetType::Camera)
	public:
		Camera() : Asset() {}
		~Camera() = default;

		void makeCulling();

		// Set entities that are to be rendered
		void setUpEntities(std::vector<Ref<Entity>>& ents) { m_drawEnts.assign(ents.begin(), ents.end()); }
		void setDepth(int32_t depth) { m_depth = depth; }

		int32_t depth() const { return m_depth; }

	private:
		std::vector<Ref<Entity>> m_drawEnts;
		int32_t m_depth{ 0 };
	};

	template<>
	bool Asset::is<Camera>() const;
}

#endif