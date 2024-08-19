#ifndef __PIONNER_GFX_PASS_RENDERPASS_H__
#define __PIONNER_GFX_PASS_RENDERPASS_H__

#include "Base.h"

namespace pio 
{
    enum RenderPassEvent
    {
        BeforeRendering = 0,
        BeforeRenderingShadows, 
        RenderingShadows, 
        AfterRenderingShadows,        
        BeforeRenderingPrepasses, 
        RenderingPrepasses, 
        AfterRenderingPrePasses,
        BeforeRenderingOpaques, 
        RenderingOpaques, 
        AfterRenderingOpaques,
        BeforeRenderingSkybox,
        RenderingSkybox, 
        AfterRenderingSkybox,
        BeforeRenderingTransparents, 
        RenderingTransparents, 
        AfterRenderingTransparents,
        BeforeRenderingPostProcessing, 
        RenderingPostProcessing, 
        AfterRenderingPostProcessing, 
        AfterRendering
    };

	class RenderPass
	{
	public:
		RenderPass(const std::string &name, RenderPassEvent event) : m_name(name), m_event(event) {}
		virtual ~RenderPass() = default;

        const std::string &name() const { return m_name; }
        RenderPassEvent event() const { return m_event; }

    public:
        static bool PassSorter(Ref<RenderPass> &lhs, Ref<RenderPass> &rhs);

    protected:
        std::string m_name;
        RenderPassEvent m_event;
	};
}

#endif