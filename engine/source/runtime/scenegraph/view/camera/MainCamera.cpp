#include "MainCamera.h"

#include "render2/ForwardRender.h"
#include "render/rhi/RhiHeader.h"

namespace pio
{
	using namespace render;

	namespace sgf
	{
		MainCamera::MainCamera() : Camera(CAM_ID_MAIN)
		{
			m_render = std::shared_ptr<Render>(new ForwardRender("Main"));
		}

		MainCamera::~MainCamera() = default;

		void MainCamera::update(const std::vector<std::shared_ptr<Layer>> &layers, RenderInfo &info)
		{
			info.viewMat  = getViewMat();
			info.prjMat   = getPrjMat();
			info.viewport = m_viewport;

			glViewport(m_viewport.x, m_viewport.y, m_viewport.width, m_viewport.height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);

			m_render->update(layers, info);
		}

		void MainCamera::release()
		{
			if (m_render)
			{
				m_render->release();
				m_render.reset();
			}
		}

	}
}