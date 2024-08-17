#ifndef __PIONNER_SCENE_SCENE_MGR_H__
#define __PIONNER_SCENE_SCENE_MGR_H__

#include "Scene.h"

namespace pio
{
	class SceneMgr
	{
	public:
		SceneMgr() {}
		~SceneMgr() = default;

		void add(const Ref<Scene>& scene);
		void remove(const Ref<Scene>& scene);
		void removeAll();

		void onUpdate(Ref<RenderContext> &context);

	private:
		std::map<AssetHandle, Ref<Scene>> m_scenes;
	};
}

#endif