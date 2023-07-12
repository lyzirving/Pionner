#include "UiPass.h"

#include "global/event/Event.h"

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
			m_ui->draw(info);
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

		bool UiPass::dispatchEvent(const Event &evt)
		{
			if (!m_ui)
				return false;

			if (evt.m_type == EVENT_TYPE_NONE)
			{
				m_ui->resetTargetView();
				return false;
			}

			if ((m_mainLayout.contains(evt.m_posX, evt.m_posY)))
			{
				/*if (m_ui->dealEvent(param, evt))
					return true;*/
			}
			m_ui->resetTargetView();

			return false;
		}

		void UiPass::setWndSize(uint32_t width, uint32_t height)
		{
			m_ui->setWndSize(width, height);
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