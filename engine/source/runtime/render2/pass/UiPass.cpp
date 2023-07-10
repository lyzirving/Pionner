#include "UiPass.h"

namespace pio
{
	namespace render
	{
		UiPass::UiPass() : RenderPass()
		{
		}

		UiPass::~UiPass() = default;

		void UiPass::tick(uint64_t deltaTimeMs)
		{
		}

		void UiPass::initializeUIRenderBackend(const std::shared_ptr<WindowUI> &ui)
		{
			m_ui = ui;
			// TODO: init ImGui, where? Think about it after the render pipeline is done.
			updateMainLayout();
		}

		void UiPass::shutdownUIRenderBackend()
		{
		}

		void UiPass::updateMainLayout()
		{
			std::shared_ptr<WindowView> view{ nullptr };
			if (m_ui && (view = m_ui->getView(UID_RENDER_PORT)))
			{
				m_mainLayout = view->getLayout();
			}
		}
	}
}