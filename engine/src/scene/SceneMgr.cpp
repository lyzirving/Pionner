#include "SceneMgr.h"
#include "Components.h"
#include "scene/3d/Camera.h"

#include "ui/LayerMgr.h"

#include "gfx/resource/RenderTarget.h"
#include "gfx/pipeline/PipelineUtils.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "SceneMgr"

namespace pio
{
	void SceneMgr::add(const Ref<Scene>& scene, bool bActive)
	{
		m_scenes[scene->assetHnd()] = scene;
		if (bActive && scene != m_active)
		{
			if (m_active) { m_active->onDetach(); }
			m_active = scene;
			m_active->onAttach();
		}
	}

	void SceneMgr::remove(const Ref<Scene>& scene)
	{
		auto it = m_scenes.find(scene->assetHnd());
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

	void pio::SceneMgr::onUpdate(Ref<RenderContext>& context, Ref<RenderPipeline>& pipeline, Ref<LayerMgr>& layerMgr)
	{
		Ref<RenderTarget> target;
		if (m_active)
		{
			auto cameraEntities = m_active->registry().view<CameraComponent>();
			auto cameras = Pipeline::FetchCamera(context, cameraEntities);
			PIO_CHECK_RETURN(!cameras.empty(), "err! camera must be added into scene before rendering");
			m_active->onUpdate(context, pipeline, cameras);
			target = cameras[0]->renderTarget();
		}
		layerMgr->onUpdate(context, target);
	}
}