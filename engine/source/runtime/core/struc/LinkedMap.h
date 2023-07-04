#ifndef __PIONNER_CORE_LINKED_MAP_H__
#define __PIONNER_CORE_LINKED_MAP_H__

#include <map>
#include <list>
#include <string>
#include <memory>

#include "core/log/LogSystem.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "LinkedMap"

namespace pio
{
	namespace core
	{
		template <class K, class T>
		class LinkedMap
		{
		public:
			typedef std::list<std::shared_ptr<T>> CoreList;
			typedef std::map<K, std::shared_ptr<T>> CoreMap;

			typedef typename std::list<std::shared_ptr<T>>::iterator   CoreListItr;
			typedef typename std::map<K, std::shared_ptr<T>>::iterator CoreMapItr;

			LinkedMap() : m_list(), m_map() {}
			~LinkedMap() { release(); }

			void add(const K &key, const std::shared_ptr<T> &val)
			{
				CoreMapItr itr = m_map.find(key);
				if (itr != m_map.end())
				{
					LOG_INFO("key[%s] already exists, ignore this one", key.c_str());
				}
				else
				{
					m_list.push_back(val);
					m_map.insert({key, val});
				}
			}

			void remove(const K &key)
			{
				CoreMapItr itr = m_map.find(key);
				if (itr == m_map.end())
				{
					return;
				}
				auto item = itr->second;

				CoreListItr listItr = m_list.begin();
				while (listItr != m_list.end())
				{
					if ((*listItr).get() == item.get())
					{
						(*listItr).reset();
						m_list.erase(listItr);
						break;
					}
					listItr++;
				}
				itr->second.reset();
				m_map.erase(itr);
			}

			bool itemExist(const K &key)
			{
				return m_map.find(key) != m_map.end();
			}

			uint32_t size()
			{
				uint32_t mapCnt = m_map.size();
				uint32_t listCnt = m_list.size();
				static_assert(mapCnt == listCnt);
				return listCnt;
			}

			bool empty()
			{
				bool mapEmpt = m_map.empty();
				bool listEmpt = m_list.empty();
				static_assert(mapEmpt == listEmpt);
				return listEmpt;
			}

			CoreListItr begin() 
			{
				return m_list.begin();
			}

			CoreListItr end()
			{
				return m_list.end();
			}

			void release()
			{
				CoreListItr itr0 = m_list.begin();
				while (itr0 != m_list.end())
				{
					(*itr0).reset();
					itr0 = m_list.erase(itr0);
				}

				CoreMapItr itr1 = m_map.begin();
				while (itr1 != m_map.end())
				{
					itr1->second.reset();
					itr1 = m_map.erase(itr1);
				}
			}

		private:

			CoreList m_list;
			CoreMap  m_map;
		};
	}
}

#endif