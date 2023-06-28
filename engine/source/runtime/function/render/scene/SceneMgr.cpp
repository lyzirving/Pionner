#include "SceneMgr.h"

#include "function/render/rhi/Rhi.h"
#include "function/render/WindowSystem.h"

#include "function/framework/comp/LightComp.h"

#include "function/render/RenderDef.h"

namespace Pionner
{
	Pionner::SceneMgr::SceneMgr()
		: m_scene(nullptr)
		, m_camera(nullptr)
		, m_frustum(nullptr)
		, m_lights()
		, m_curLight(LIGHT_TYPE_DIRECTIONAL)
	{
	}

	SceneMgr::~SceneMgr() = default;

	void SceneMgr::initialize(SceneMgrInitInfo &info)
	{
		m_scene = std::make_shared<RenderScene>();
		m_scene->initialize(info.rhi);

		m_camera = std::make_shared<Camera>();
		m_camera->setPosition(glm::vec3(5.f, 5.f, 5.f));

		m_frustum = std::make_shared<Frustum>();
		float aspect = float(info.window->getWidth()) / float(info.window->getHeight());
		m_frustum->setAspect(aspect);
	}

	void SceneMgr::swap(const LightComp &comp, const RenderParam &param)
	{
		m_curLight = comp.m_type;
		if (!m_lights[m_curLight])
		{
			m_lights[m_curLight] = Light::createLight(m_curLight);
			m_lights[m_curLight]->initDepthBuffer(1024, 1024, param);
		}
		m_lights[m_curLight]->setPosition(comp.m_pos);
		m_lights[m_curLight]->setDirection(comp.m_dir);
		m_lights[m_curLight]->setColor(comp.m_ka, comp.m_kd, comp.m_ks);
		m_lights[m_curLight]->setIntensity(comp.m_ia, comp.m_id, comp.m_is);
		m_lights[m_curLight]->setShininess(comp.m_shininess);

		m_lights[m_curLight]->setNear(m_frustum->near());
		m_lights[m_curLight]->setFar(m_frustum->far());
	}

	void SceneMgr::shutdown()
	{
		if (m_frustum)
		{
			m_frustum.reset();
		}

		if (m_camera)
		{
			m_camera.reset();
		}

		if (m_scene)
		{
			m_scene->shutdown();
			m_scene.reset();
		}

		for (size_t i = 0; i < LIGNT_TYPE_COUNT; i++)
		{
			m_lights[i].reset();
		}
	}

	void SceneMgr::resize(int width, int height)
	{
		float aspect = float(width) / float(height);
		m_frustum->setAspect(aspect);
	}

}