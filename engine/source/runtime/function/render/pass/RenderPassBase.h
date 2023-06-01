#ifndef __RENDER_PASS_BASE_H__
#define __RENDER_PASS_BASE_H__

#include "function/render/RenderDef.h"

namespace Pionner
{
	class SceneMgr;

	class RenderPassBase
	{
	public:
		RenderPassBase();
		virtual ~RenderPassBase();

		virtual void initialize(const RenderPassInitInfo &info) = 0;
		virtual void shutdown() = 0;
		virtual void draw(std::shared_ptr<SceneMgr> &sceneMgr) = 0;

		virtual void setPassCommonInfo(RenderPassCommonInfo &info);

	protected:
		std::shared_ptr<Rhi> m_rhi;
	};
}

#endif