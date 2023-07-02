#ifndef __RENDER_SYSTEM_H__
#define __RENDER_SYSTEM_H__

#include "function/SystemBase.h"
#include "function/render/RenderDef.h"
#include "function/render/resource/ResourceDef.h"

namespace Pionner
{
	class  SceneMgr;
	class  ShaderMgr;
	class  RenderResourceMgr;

	class RenderSystem : public SystemBase
	{
	public:
		RenderSystem(const std::shared_ptr<World> &world);
		virtual ~RenderSystem();

		inline std::shared_ptr<RenderResourceMgr> getResourceMgr() { return m_resourceMgr; }

		void initialize(RenderSystemInitInfo &info);
		void initializeUIRenderBackend(const std::shared_ptr<WindowUI> &ui);
		void shutdownUIRenderBackend();

		void notifyRelease(BufferType type, uint32_t slot);

		void swapData(float deltaTime);
		void shutdown();

		virtual void tick(float deltaTime) override;

	private:
		std::shared_ptr<Rhi>                m_rhi;
		std::shared_ptr<RenderPipeline>     m_pipeLine;
		std::shared_ptr<SceneMgr>           m_sceneMgr;
		std::shared_ptr<ShaderMgr>          m_shaderMgr;
		std::shared_ptr<RenderResourceMgr>  m_resourceMgr;
	};
}

#endif