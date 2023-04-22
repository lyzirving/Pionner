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

	private:
		using Buffer = std::shared_ptr<GfxBuffer>;

		class BufferArray
		{
		public:
			BufferArray(const std::shared_ptr<Rhi> &rhi);
			~BufferArray();

			Buffer   allocate(BufferType type);
			bool     empty();
			Buffer   find(uint32_t id);
			uint32_t size();

		private:
			static Buffer createBuffer(BufferType type);

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
	};
}

#endif