#ifndef __RENDER_RESOURCE_BUFFER_ARRAY_H__
#define __RENDER_RESOURCE_BUFFER_ARRAY_H__

#include <vector>
#include <list>

#include "function/render/resource/ResourceDef.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "BufferArray"

namespace Pionner
{
	class Rhi;
	class RenderResourceMgr;

	template<class T>
	class BufferArray
	{
	public:
		BufferArray(const std::shared_ptr<Rhi> &rhi)
			: m_rhi(rhi), m_active(), m_abandoned(), m_availableSlots()
		{
		}

		~BufferArray()
		{
			m_rhi.reset();
		}

		std::shared_ptr<T> allocate(BufferType type, std::shared_ptr<RenderResourceMgr> &mgr)
		{
			std::shared_ptr<T> ret{ nullptr };
			if (m_availableSlots.empty())
			{
				uint32_t slot = m_active.size();
				ret = createBuffer(type, mgr);
				ret->m_slot = slot;
				m_active.push_back(ret);
			}
			else
			{
				uint32_t availableSlot = m_availableSlots.back();
				m_availableSlots.pop_back();

				uint32_t size = m_active.size();
				if (availableSlot >= size)
				{
					LOG_ERR("available slot is invalid, slot[%u] >= size[%u], ignore the slot", availableSlot, size);
					uint32_t slot = m_active.size();
					ret = createBuffer(type, mgr);
					ret->m_slot = slot;
					m_active.push_back(ret);
				}
				else
				{
					if (m_active[availableSlot].get())
					{
						LOG_ERR("buffer exists in slot[%u], release it", availableSlot);
						m_abandoned.push_back(m_active[availableSlot]);
					}
					m_active[availableSlot] = createBuffer(type, mgr);
					m_active[availableSlot]->m_slot = availableSlot;
					ret = m_active[availableSlot];
				}
			}
			return ret;
		}

		std::shared_ptr<T> find(uint32_t slot)
		{
			std::shared_ptr<T> ret{ nullptr };
			if (exist(slot))
			{
				ret = m_active[slot];
			}
			return ret;
		}

		void checkAbandoned()
		{
			if (!m_abandoned.empty())
			{
				auto itr = m_abandoned.begin();
				while (itr != m_abandoned.end())
				{
					(*itr)->deleteResource();
					(*itr).reset();
					itr = m_abandoned.erase(itr);
				}
			}
		}

		void clearActive()
		{
			for (auto &active : m_active)
			{
				if (active)
				{
					active->deleteResource();
					active.reset();
				}
			}
			std::vector<std::shared_ptr<T>>().swap(m_active);
		}

		uint32_t size() { return m_active.size(); }

		bool empty() { return m_active.empty(); }

		bool exist(uint32_t slot)
		{
			return slot < m_active.size() && m_active[slot].get();
		}

		void release(uint32_t slot)
		{
			if (exist(slot))
			{
				auto abandoned = m_active[slot];
				m_abandoned.push_back(abandoned);
				m_availableSlots.push_back(abandoned->m_slot);
				m_active[slot].reset();
			}
		}

	protected:
		std::shared_ptr<T> createBuffer(BufferType type, std::shared_ptr<RenderResourceMgr> &mgr);

	private:
		std::shared_ptr<Rhi>            m_rhi;
		std::vector<std::shared_ptr<T>> m_active;
		std::list<std::shared_ptr<T>>   m_abandoned;

		std::list<uint32_t>  m_availableSlots;
	};

	template<>
	std::shared_ptr<GfxBuffer> BufferArray<GfxBuffer>::createBuffer(BufferType type, std::shared_ptr<RenderResourceMgr> &mgr);

	template<>
	std::shared_ptr<GfxFrameBuffer> BufferArray<GfxFrameBuffer>::createBuffer(BufferType type, std::shared_ptr<RenderResourceMgr> &mgr);
}

#endif