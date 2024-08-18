#include "SceneMgr.h"

#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "SceneMgr"

namespace pio
{
	void SceneMgr::add(const Ref<Scene>& scene)
	{
		auto it = m_scenes.find(scene->id());
		if (it == m_scenes.end())
		{
			scene->onAttach();
			m_scenes[scene->id()] = scene;			
		}
	}

	void SceneMgr::remove(const Ref<Scene>& scene)
	{
		auto it = m_scenes.find(scene->id());
		if (it != m_scenes.end())
		{
			scene->onDetach();
			m_scenes.erase(it);
		}
	}

	void SceneMgr::removeAll()
	{
		auto it = m_scenes.begin();
		while (it != m_scenes.end())
		{
			it->second->onDetach();
			it = m_scenes.erase(it);
		}
	}

	void pio::SceneMgr::onUpdate(Ref<RenderContext>& context, Ref<RenderPipeline>& pipeline)
	{
		for (auto &it : m_scenes)
		{
			it.second->onUpdate(context, pipeline);
		}
	}
}