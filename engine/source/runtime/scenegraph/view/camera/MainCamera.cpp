#include "MainCamera.h"

#include "render2/ForwardRender.h"

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