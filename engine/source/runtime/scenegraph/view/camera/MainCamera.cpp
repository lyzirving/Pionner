#include "MainCamera.h"

#include "render2/ForwardRender.h"

#include "gfx/context/GraphicContext.h"

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

			auto rhi = info.gfxContext->getRhi();
			auto state = info.gfxContext->getState();

			rhi->setViewport(m_viewport.x, m_viewport.y, m_viewport.width, m_viewport.height);
			gfx::ClearMode mode = gfx::ClearMode::common(m_clearColor);
			state->setClearMode(mode);

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