#include "UiPass.h"

namespace pio
{
	namespace render
	{
		UiPass::UiPass() : RenderPass()
		{
		}

		UiPass::~UiPass() = default;

		void UiPass::update(sgf::RenderInfo &info)
		{
		}

		void UiPass::release()
		{
			m_ui.reset();
		}

		void UiPass::attachUi(const std::shared_ptr<WindowUI> &ui)
		{
			m_ui = ui;
			updateMainLayout();
		}

		void UiPass::setWndSize(uint32_t width, uint32_t height)
		{
			if (m_ui) m_ui->resize(width, height);
			updateMainLayout();
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