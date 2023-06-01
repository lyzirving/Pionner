#ifndef __UI_PASS_H__
#define __UI_PASS_H__

#include "function/ui/WindowView.h"
#include "function/render/pass/RenderPassBase.h"

namespace Pionner
{
	class UIPass : public RenderPassBase
	{
	public:
		UIPass();
		virtual ~UIPass();

		virtual void initialize(const RenderPassInitInfo &info) override;
		virtual void shutdown() override;
		virtual void draw(std::shared_ptr<SceneMgr> &sceneMgr) override;

		ViewLayout getRenderportLayout();
		void initializeUIRenderBackend(const std::shared_ptr<WindowUI> &ui);
		void shutdownUIRenderBackend();

		void resize(int width, int height);

	private:
		void drawUI(std::shared_ptr<SceneMgr> &sceneMgr);
		void drawLeftPanel(std::shared_ptr<SceneMgr> &sceneMgr);
		void drawRightPanel(std::shared_ptr<SceneMgr> &sceneMgr);
		void drawBottomPanel(std::shared_ptr<SceneMgr> &sceneMgr);

	private:
		std::shared_ptr<WindowUI> m_ui;
	};
}

#endif