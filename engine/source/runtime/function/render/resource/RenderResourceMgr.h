#ifndef __RENDER_RESOURCE_MGR_H__
#define __RENDER_RESOURCE_MGR_H__

#include <vector>
#include <list>
#include <memory>

#include "function/render/resource/ResourceDef.h"

namespace Pionner
{
	class Rhi;
	class GfxBuffer;

	class RenderResourceMgr
	{
	public:
		RenderResourceMgr(const std::shared_ptr<Rhi> &rhi);
		~RenderResourceMgr();

		using Buffer = std::shared_ptr<GfxBuffer>;

		inline std::shared_ptr<Rhi> getRhi() const { return m_rhi; };

		Buffer allocate(BufferType type);
		void checkAbandoned();
		void deleteResource(DataType type, uint32_t slot);
		Buffer find(DataType type, uint32_t slot);
		void makeSelfWeak(const std::shared_ptr<RenderResourceMgr> &self);
		void release(BufferType type, uint32_t slot);
		void shutdown();

	private:
		class BufferArray
		{
		public:
			BufferArray(const std::shared_ptr<Rhi> &rhi);
			~BufferArray();

			Buffer   allocate(BufferType type, std::shared_ptr<RenderResourceMgr> &mgr);
			void     checkAbandoned();
			void     clearActive();
			bool     empty();
			bool     exist(uint32_t slot);
			Buffer   find(uint32_t slot);
			void     release(uint32_t slot);
			uint32_t size();

		private:
			static Buffer createBuffer(BufferType type, std::shared_ptr<RenderResourceMgr> &mgr);

		private:
			std::shared_ptr<Rhi> m_rhi;
			std::vector<Buffer>  m_activeBuffers;
			std::list<Buffer>    m_abandoned;

			std::list<uint32_t>  m_availableSlots;
		};

	private:

		std::shared_ptr<Rhi> m_rhi;
		BufferArray          m_vertexArray;
		BufferArray          m_indiceArray;
		BufferArray          m_textureArray;

		std::weak_ptr<RenderResourceMgr> m_weakSelf;
	};
}

#define RESOURCE_MGR_MAKE_SELF_WEAK(shared_self)                  \
        {							                              \
			std::shared_ptr<RenderResourceMgr> obj = shared_self; \
            shared_self->makeSelfWeak(obj);                       \
		}

#endif