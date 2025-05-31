#pragma once

#include "Common.h"
#include "gfx/rhi/RenderStateAttrs.h"

namespace pio
{
	enum RenderPassEvent
	{
		BeforeRendering                = 50,									   
		BeforeRenderingShadows         = 100,
		RenderingShadows,			   
		AfterRenderingShadows		   = 149,									   
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
		
	class CameraNode;
	class FrameBuffer;
	class RenderContext;

	class RenderPass : public Object
	{
		RTTR_ENABLE(Object)
	public:
		RenderPass(const std::string& name, RenderPassEvent event) : m_Name(name), m_Event(event) {}
		virtual ~RenderPass() = default;				

		virtual void OnAttach(const Ref<RenderContext>& context) {}
		virtual void OnDetach(const Ref<RenderContext>& context) {}
		virtual void OnExecute(const Ref<RenderContext>& context, const Ref<CameraNode>& camera) {}

	public:
		void SetActive(bool val) { m_bActive = val; }		

		const std::string& Name() const { return m_Name; }		
		bool bActive() const { return m_bActive; }
		RenderPassEvent GetEvent() const { return m_Event; }
		Ref<FrameBuffer>& GetFrameBuffer() { return m_FrameBuff; }
		const Ref<FrameBuffer>& GetFrameBuffer() const { return m_FrameBuff; }
		RenderStateAttrs& GetRenderState() { return m_Attrs; }
		const RenderStateAttrs& GetRenderState() const { return m_Attrs; }

	public:
		static bool PassSorter(Ref<RenderPass>& lhs, Ref<RenderPass>& rhs);

	protected:
		std::string m_Name;		
		RenderPassEvent m_Event;
		bool m_bActive{ true };

		Ref<FrameBuffer> m_FrameBuff;
		RenderStateAttrs m_Attrs;
	};

	struct BlockRange
	{
		RenderPassEvent Left{ BeforeRendering }, Right{ BeforeRendering };

		BlockRange() {}
		BlockRange(RenderPassEvent l, RenderPassEvent r) : Left(l), Right(r) {}

		bool Contains(const Ref<RenderPass>& pass) const { return pass->GetEvent() >= Left && pass->GetEvent() < Right; }
		bool Intersect(const std::vector<Ref<RenderPass>> &passes) const
		{
			if(passes.empty()) return false;
			if(passes.size() == 1) return Contains(passes[0]);

			RenderPassEvent l = passes[0]->GetEvent();
			RenderPassEvent r = passes[passes.size() - 1]->GetEvent();
			RenderPassEvent lMax = Left > l ? Left : l;
			RenderPassEvent rMin = Right > r ? r : Right;
			return lMax <= rMin;
		}
	};

	class RenderBlock
	{
	public:
		static BlockRange GetBlockRange(RenderBlockFlags flag);
	};
}