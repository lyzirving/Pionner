#include "function/render/resource/RenderResourceMgr.h"
#include "function/render/resource/buffer/GfxBuffer.h"
#include "function/render/rhi/opengl/buffer/VertexBuffer.h"

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
		m_rhi.reset();
		m_weakSelf.reset();
	}

	RenderResourceMgr::Buffer RenderResourceMgr::allocate(BufferType type)
	{
		Buffer ret{ nullptr };
		std::shared_ptr<RenderResourceMgr> mgr = m_weakSelf.lock();
		if (mgr)
		{
			switch (type)
			{
				case Pionner::BUF_MEM_ARRAY:
					ret = m_vertexArray.allocate(type, mgr);
					break;
				case Pionner::BUF_VBO:
					break;
				case Pionner::BUF_EBO:
					break;
				case Pionner::BUF_CNT:
					break;
				default:
					break;
			}
		}
		return ret;
	}

	void RenderResourceMgr::release(BufferType type, uint32_t id)
	{
		switch (type)
		{
			case Pionner::BUF_MEM_ARRAY:
			{
				if (m_vertexArray.exist(id))
				{
					m_vertexArray.release(id);
				}
				break;
			}
			case Pionner::BUF_VBO:
				break;
			case Pionner::BUF_EBO:
				break;
			case Pionner::BUF_CNT:
				break;
			default:
				break;
		}
	}

	void RenderResourceMgr::makeSelfWeak(const std::shared_ptr<RenderResourceMgr> &self)
	{
		m_weakSelf = self;
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
		m_rhi.reset();
	}

	RenderResourceMgr::Buffer RenderResourceMgr::BufferArray::allocate(BufferType type, std::shared_ptr<RenderResourceMgr> &mgr)
	{
		Buffer ret{ nullptr };
		if (m_availableSlots.empty())
		{
			uint32_t slot = m_activeBuffers.size();
			ret = createBuffer(type, mgr);
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
				ret = createBuffer(type, mgr);
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
				m_activeBuffers[availableSlot] = createBuffer(type, mgr);
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

	bool RenderResourceMgr::BufferArray::exist(uint32_t id)
	{
		return id < m_activeBuffers.size() && m_activeBuffers[id].get();
	}

	RenderResourceMgr::Buffer RenderResourceMgr::BufferArray::find(uint32_t id)
	{
		Buffer ret{ nullptr };
		if (exist(id))
		{
			ret = m_activeBuffers[id];
		}
		return ret;
	}

	void RenderResourceMgr::BufferArray::release(uint32_t id)
	{
		if (exist(id))
		{
			Buffer abandoned = m_activeBuffers[id];
			m_abandoned.push_back(abandoned);
			m_availableSlots.push_back(abandoned->m_id);
			m_activeBuffers[id].reset();
		}
	}

	uint32_t RenderResourceMgr::BufferArray::size()
	{
		return m_activeBuffers.size();
	}

	RenderResourceMgr::Buffer RenderResourceMgr::BufferArray::createBuffer(BufferType type, std::shared_ptr<RenderResourceMgr> &mgr)
	{
		return Buffer();
	}

}