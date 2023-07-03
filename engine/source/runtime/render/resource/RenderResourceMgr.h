#ifndef __RENDER_RESOURCE_MGR_H__
#define __RENDER_RESOURCE_MGR_H__

#include "tool/BufferArray.h"

namespace pio
{
	class Rhi;

	class RenderResourceMgr
	{
	public:
		RenderResourceMgr(const std::shared_ptr<Rhi> &rhi);
		~RenderResourceMgr();

		inline std::shared_ptr<Rhi> getRhi() const { return m_rhi; };

		std::shared_ptr<GfxBuffer>      allocate(BufferType type);
		std::shared_ptr<GfxFrameBuffer> allocFbo(BufferType type);

		std::shared_ptr<GfxBuffer>      createHolderBuffer(BufferType type);

		std::shared_ptr<GfxBuffer>      find(BufferType type, uint32_t slot);
		std::shared_ptr<GfxFrameBuffer> findFbo(uint32_t slot);

		void checkAbandoned();
		void makeSelfWeak(const std::shared_ptr<RenderResourceMgr> &self);

		void notifyRelease(BufferType type, uint32_t slot);

		void shutdown();

	private:
		std::shared_ptr<Rhi>   m_rhi;
		BufferArray<GfxBuffer> m_vertexArray;
		BufferArray<GfxBuffer> m_indiceArray;
		BufferArray<GfxBuffer> m_textureArray;
		BufferArray<GfxFrameBuffer> m_frameBuffers;

		std::weak_ptr<RenderResourceMgr> m_weakSelf;
	};
}

#define RESOURCE_MGR_MAKE_SELF_WEAK(shared_self)                  \
        {							                              \
			std::shared_ptr<RenderResourceMgr> obj = shared_self; \
            shared_self->makeSelfWeak(obj);                       \
		}

#endif