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

		virtual void initialize(const RenderPassInitInfo& info) override;
		virtual void shutdown() override;

		virtual void initializeUIRenderBackend(WindowUI* ui) override;
		virtual void shutdownUIRenderBackend() override;
		virtual void draw() override;

	private:
		WindowUI* m_ui;
	};
}

#endif