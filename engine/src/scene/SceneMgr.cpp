#include "SceneMgr.h"

#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "SceneMgr"

namespace pio
{
	void SceneMgr::add(const Ref<Scene>& scene, bool bActive)
	{
		m_scenes[scene->id()] = scene;
		if (bActive && scene != m_active)
		{
			if (m_active) { m_active->onDetach(); }
			m_active = scene;
			m_active->onAttach();
		}
	}

	void SceneMgr::remove(const Ref<Scene>& scene)
	{
		auto it = m_scenes.find(scene->id());
		if (it != m_scenes.end())
		{
			if (m_active == scene)
			{
				m_active->onDetach();
				m_active.reset();
			}
			m_scenes.erase(it);
		}
	}

	void SceneMgr::removeAll()
	{
		auto it = m_scenes.begin();
		while (it != m_scenes.end())
		{
			if (m_active == it->second)
			{
				m_active->onDetach();
				m_active.reset();
			}
			it = m_scenes.erase(it);
		}
	}

	void pio::SceneMgr::onUpdate(Ref<RenderContext>& context, Ref<RenderPipeline>& pipeline)
	{
		if (m_active)
		{
			m_active->onUpdate(context, pipeline);
		}
	}
}