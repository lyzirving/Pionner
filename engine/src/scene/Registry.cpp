#include "Registry.h"

#include "Components.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Registry"

namespace pio
{
	PIO_SINGLETON_IMPL(Registry);

	uint32_t Registry::k_entNum = 0;

	Registry::Registry()
	{
	}

	Registry::~Registry()
	{
		auto it = m_entities.begin();
		while (it != m_entities.end())
		{
			if ((*it))
			{
				m_ECSImpl.destroy((*it)->m_handle);
				(*it).reset();
			}
			it = m_entities.erase(it);
		}
	}

	void Registry::Init()
	{
	}

	void Registry::Shutdown()
	{
		{
			std::lock_guard<std::mutex> lk{ s_mutex };
			delete s_instance;
			s_instance = nullptr;
		}
	}

	void Registry::destroy(Entity &entity)
	{
		uint32_t index = entity.m_handle.index;
		if (index < m_entities.size())
		{
			//NOTE: we should not erase the slot due to the third lib decs
			m_entities[index].reset();
		}
		else
		{
			LOGW("invalid entity, index[%u]", index);
		}
		m_ECSImpl.destroy(entity.m_handle);
	}

	void Registry::cacheEntity(const Ref<Entity> &entity)
	{
		uint32_t index = entity->m_handle.index;
		uint32_t cacheSize = m_entities.size();
		if (index < cacheSize)
		{
			//LOGD("cache entity, index[%u]", index);
			m_entities[index] = entity;
		}
		else if (index == cacheSize)
		{
			//LOGD("push back entity, index[%u]", index);
			m_entities.push_back(entity);
		}
		else
		{
			LOGE("invalid state, new entity index[%u], entity cache size[%u]", index, cacheSize);
			m_entities.resize(index + 1);
			m_entities[index] = entity;
		}
	}
}