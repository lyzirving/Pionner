#ifndef __PIONNER_GFX_PASS_RENDERPASS_H__
#define __PIONNER_GFX_PASS_RENDERPASS_H__

#include "Base.h"

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
	};

	struct BlockRange
	{
		RenderPassEvent Left{ BeforeRendering }, Right{ BeforeRendering };

		BlockRange() {}
		BlockRange(RenderPassEvent l, RenderPassEvent r) : Left(l), Right(r) {}

		bool contains(Ref<RenderPass>& pass) { return pass->event() >= Left && pass->event() < Right; }
	};

	class RenderBlock
	{
	public:
		static BlockRange GetBlockRange(RenderBlockFlags flag);
	};
}

#endif