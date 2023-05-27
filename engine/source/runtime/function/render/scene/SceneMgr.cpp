#include "function/render/scene/SceneMgr.h"

#include "function/render/rhi/Rhi.h"
#include "function/render/WindowSystem.h"

namespace Pionner
{
	Pionner::SceneMgr::SceneMgr()
		: m_scene(nullptr)
		, m_layout(nullptr)
		, m_camera(nullptr)
		, m_frustum(nullptr)
	{
	}

	SceneMgr::~SceneMgr() = default;

	void SceneMgr::initialize(SceneMgrInitInfo &info)
	{
		m_layout = std::shared_ptr<Layout>(new Layout);
		m_layout->initialize(info.window);

		m_scene = std::make_shared<RenderScene>();
		m_scene->initialize(info.rhi);

		m_camera = std::make_shared<Camera>();
		m_camera->setPosition(glm::vec3(5.f, 4.f, 5.f));

		m_frustum = std::make_shared<Frustum>();
		float aspect = float(m_layout->m_drawPanelInfo.m_width) / float(m_layout->m_drawPanelInfo.m_height);
		m_frustum->setAspect(aspect);
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

		if (m_layout)
		{
			m_layout.reset();
		}

		if (m_scene)
		{
			m_scene->shutdown();
			m_scene.reset();
		}
	}

	void SceneMgr::reviseLayout(int width, int height)
	{
		if (m_layout->revise(width, height))
		{
			float aspect = float(m_layout->m_drawPanelInfo.m_width) / float(m_layout->m_drawPanelInfo.m_height);
			m_frustum->setAspect(aspect);
		}
	}

}