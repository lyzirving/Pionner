#ifndef __PIONNER_SCENE_SCENE_MGR_H__
#define __PIONNER_SCENE_SCENE_MGR_H__

#include "Scene.h"

namespace pio
{
	class LayerMgr;
	class RenderTarget;

	class SceneMgr
	{
	public:
		SceneMgr() {}
		~SceneMgr() = default;

		void add(const Ref<Scene>& scene, bool bActive = false);
		void remove(const Ref<Scene>& scene);
		void removeAll();

		void onDetach();
		void onUpdate(Ref<RenderContext>& context, Ref<RenderPipeline>& pipeline, Ref<LayerMgr>& layerMgr);

	private:
		std::map<UUID32, Ref<Scene>> m_scenes;
		Ref<Scene> m_active;
		Ref<RenderTarget> m_target;
	};
}

#endif