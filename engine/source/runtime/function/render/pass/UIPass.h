#ifndef __UI_PASS_H__
#define __UI_PASS_H__

#include "function/ui/WindowView.h"
#include "RenderPassBase.h"

namespace Pionner
{
	class Event;

	class UIPass : public RenderPassBase
	{
	public:
		UIPass();
		virtual ~UIPass();

		virtual void initialize(const RenderPassInitInfo &info) override;
		virtual void shutdown() override;
		virtual void draw(RenderParam &param) override;

		inline ViewLayout getRenderLayout() { return m_renderLayout; }

		void initializeUIRenderBackend(const std::shared_ptr<WindowUI> &ui);
		bool dealEvent(RenderParam &param, const Event &evt);
		void shutdownUIRenderBackend();

		void resize(int width, int height);

	private:
		void calcLayout();

	private:
		std::shared_ptr<WindowUI> m_ui;
		int32_t m_windowWidth, m_windowHeight;
		ViewLayout m_renderLayout;
	};
}

#endif