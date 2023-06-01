#ifndef __UI_PASS_H__
#define __UI_PASS_H__

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

		virtual void initializeUIRenderBackend(WindowUI *ui) override;
		virtual void shutdownUIRenderBackend() override;

		void resize(int width, int height);

	private:
		void drawUI(std::shared_ptr<SceneMgr> &sceneMgr);
		void drawLeftPanel(std::shared_ptr<SceneMgr> &sceneMgr);
		void drawRightPanel(std::shared_ptr<SceneMgr> &sceneMgr);
		void drawBottomPanel(std::shared_ptr<SceneMgr> &sceneMgr);

	private:
		WindowUI *m_ui;
	};
}

#endif