#ifndef __PIONNER_RENDER_PASS_UIPASS_H__
#define __PIONNER_RENDER_PASS_UIPASS_H__

#include "global/window/ui/WindowUI.h"
#include "RenderPass.h"

namespace pio
{
	class Event;

	namespace render
	{
		class UiPass : public RenderPass
		{
		public:
			UiPass();
			virtual ~UiPass();

			virtual void update(sgf::RenderInfo &info) override;
			virtual void release() override;

			void attachUi(const std::shared_ptr<WindowUI> &ui);
			bool dispatchEvent(const Event &evt);
			void setWndSize(uint32_t width, uint32_t height);

			inline const ViewLayout &getMainLayout() { return m_mainLayout; }

		private:
			void updateMainLayout();

		private:
			std::shared_ptr<WindowUI> m_ui{ nullptr };
			ViewLayout                m_mainLayout{};
		};
	}
}

#endif	