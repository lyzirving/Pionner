#include "function/render/resource/RenderResourceMgr.h"
#include "function/render/resource/buffer/GfxBuffer.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderResourceMgr"

namespace Pionner
{
	RenderResourceMgr::RenderResourceMgr(const std::shared_ptr<Rhi> &rhi)
		: m_rhi(rhi)
		, m_vertexArray(rhi)
		, m_indiceArray(rhi)
	{
	}

	RenderResourceMgr::~RenderResourceMgr()
	{
	}

	RenderResourceMgr::BufferArray::BufferArray(const std::shared_ptr<Rhi> &rhi)
		: m_rhi(rhi)
		, m_activeBuffers()
		, m_abandoned()
		, m_availableSlots()
	{
	}

	RenderResourceMgr::BufferArray::~BufferArray()
	{
	}

	RenderResourceMgr::Buffer RenderResourceMgr::BufferArray::allocate(BufferType type)
	{
		Buffer ret{ nullptr };
		if (m_availableSlots.empty())
		{
			uint32_t slot = m_activeBuffers.size();
			ret = createBuffer(type);
			ret->m_id = slot;
			m_activeBuffers.push_back(ret);
		}
		else
		{
			uint32_t availableSlot = m_availableSlots.back();
			m_availableSlots.pop_back();

			uint32_t size = m_activeBuffers.size();
			if (availableSlot >= size)
			{
				LOG_ERR("available slot is invalid, slot[%u] >= size[%u], ignore the slot", availableSlot, size);
				uint32_t slot = m_activeBuffers.size();
				ret = createBuffer(type);
				ret->m_id = slot;
				m_activeBuffers.push_back(ret);
			}
			else
			{
				if (m_activeBuffers[availableSlot].get())
				{
					LOG_ERR("buffer already exists in slot[%u], release it", availableSlot);
					m_abandoned.push_back(m_activeBuffers[availableSlot]);
				}
				m_activeBuffers[availableSlot] = createBuffer(type);
				m_activeBuffers[availableSlot]->m_id = availableSlot;
				ret = m_activeBuffers[availableSlot];
			}
		}
		return ret;
	}

	bool RenderResourceMgr::BufferArray::empty()
	{
		return m_activeBuffers.empty();
	}

	RenderResourceMgr::Buffer RenderResourceMgr::BufferArray::find(uint32_t id)
	{
		return Buffer();
	}

	uint32_t RenderResourceMgr::BufferArray::size()
	{
		return m_activeBuffers.size();
	}

	RenderResourceMgr::Buffer RenderResourceMgr::BufferArray::createBuffer(BufferType type)
	{
		return Buffer();
	}

}