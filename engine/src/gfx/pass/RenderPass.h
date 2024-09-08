#ifndef __PIONNER_GFX_PASS_RENDERPASS_H__
#define __PIONNER_GFX_PASS_RENDERPASS_H__

#include "Entry.h"
#include "gfx/rhi/RenderStateAttrs.h"

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

	enum class RenderPassType : uint8_t
	{
		MainLightShadowCaster, GBuffer, Deferred, Num
	};

	class RenderContext;
	class FrameBuffer;
	class Camera;

	#define OVERRIDE_PASS_TYPE(TypeName)  public:\
									      static  RenderPassType StaticType() { return TypeName; }\
									      virtual RenderPassType type() const override { return StaticType(); }

	class RenderPass
	{
		PIO_IS_AS_INTERFACE_DECLARE(RenderPass)
	public:
		RenderPass(const std::string& name, RenderPassEvent event) : m_name(name), m_event(event) {}
		virtual ~RenderPass() = default;				

		virtual void onAttach(Ref<RenderContext>& context) {}
		virtual void onDetach(Ref<RenderContext>& context) {}
		virtual void onExecute(Ref<RenderContext> &context, Ref<Camera>&camera, Ref<RenderPass>& lastPass) {}
		virtual RenderPassType type() const { return RenderPassType::Num; }

	public:
		void setActive(bool val) { m_bActive = val; }

		Ref<FrameBuffer>& frameBuffer() { return m_frameBuff; }

		const std::string& name() const { return m_name; }
		RenderPassEvent event() const { return m_event; }
		bool bActive() const { return m_bActive; }
		const Ref<FrameBuffer>& frameBuffer() const { return m_frameBuff; }

	public:
		static bool PassSorter(Ref<RenderPass>& lhs, Ref<RenderPass>& rhs);

	protected:
		std::string m_name;		
		RenderPassEvent m_event;
		bool m_bActive{ true };

		Ref<FrameBuffer> m_frameBuff;
		RenderStateAttrs m_attrs;
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