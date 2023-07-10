#ifndef __PIONNER_RENDER_PASS_UIPASS_H__
#define __PIONNER_RENDER_PASS_UIPASS_H__

#include "global/window/ui/WindowUI.h"
#include "RenderPass.h"

namespace pio
{
	namespace render
	{
		class UiPass : public RenderPass
		{
		public:
			UiPass();
			virtual ~UiPass();

			virtual void tick(uint64_t deltaTimeMs) override;

			void initializeUIRenderBackend(const std::shared_ptr<WindowUI> &ui);
			void shutdownUIRenderBackend();

		private:
			void updateMainLayout();

		private:
			std::shared_ptr<WindowUI> m_ui{ nullptr };
			ViewLayout                m_mainLayout{};
		};
	}
}

#endif	