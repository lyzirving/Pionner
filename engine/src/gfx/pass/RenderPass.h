#ifndef __PIONNER_GFX_PASS_RENDERPASS_H__
#define __PIONNER_GFX_PASS_RENDERPASS_H__

#include "Entry.h"

namespace pio
{
	enum RenderPassEvent
	{
		BeforeRendering                = 50,									   
		BeforeRenderingShadows         = 100,
		RenderingShadows,			   
		AfterRenderingShadows,		   									   
		BeforeRenderingPrepasses       = 150,
		RenderingPrepasses,			   
		AfterRenderingPrePasses,	   									   
		BeforeRenderingOpaques         = 200,
		RenderingOpaques,			   
		AfterRenderingOpaques,		   									   
		BeforeRenderingSkybox          = 250,
		RenderingSkybox,			   
		AfterRenderingSkybox,		   									   
		BeforeRenderingTransparents    = 300,
		RenderingTransparents,		   
		AfterRenderingTransparents,								   
		BeforeRenderingPostProcessing  = 350,
		RenderingPostProcessing,	   
		AfterRenderingPostProcessing,  									   
		AfterRendering                 = 400
	};

	enum RenderBlockFlags
	{
		MainBeforeRendering = 0, 
		MainRenderingOpaque,
		MainRenderingTransparents,
		MainAfterRendering
	};

	class RenderContext;
	class FrameBuffer;

	class RenderPass
	{
	public:
		RenderPass(const std::string& name, RenderPassEvent event) : m_name(name), m_event(event) {}
		virtual ~RenderPass() = default;

		const std::string& name() const { return m_name; }
		RenderPassEvent event() const { return m_event; }

		virtual void execute(Ref<RenderContext> &context) {}

	public:
		static bool PassSorter(Ref<RenderPass>& lhs, Ref<RenderPass>& rhs);

	protected:
		std::string m_name;
		RenderPassEvent m_event;
		Ref<FrameBuffer> m_frameBuff;
	};

	struct BlockRange
	{
		RenderPassEvent Left{ BeforeRendering }, Right{ BeforeRendering };

		BlockRange() {}
		BlockRange(RenderPassEvent l, RenderPassEvent r) : Left(l), Right(r) {}

		bool contains(Ref<RenderPass>& pass) const { return pass->event() >= Left && pass->event() < Right; }
		bool intersect(std::vector<Ref<RenderPass>> &passes) const
		{
			if(passes.empty()) return false;
			if(passes.size() == 1) return contains(passes[0]);

			RenderPassEvent l = passes[0]->event();
			RenderPassEvent r = passes[passes.size() - 1]->event();
			RenderPassEvent lMax = Left > l ? Left : l;
			RenderPassEvent rMin = Right > r ? r : Right;
			return lMax < rMin;
		}
	};

	class RenderBlock
	{
	public:
		static BlockRange GetBlockRange(RenderBlockFlags flag);
	};
}

#endif