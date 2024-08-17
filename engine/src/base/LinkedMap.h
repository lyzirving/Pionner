#ifndef __PIONNER_BASE_LINKED_MAP_H__
#define __PIONNER_BASE_LINKED_MAP_H__

#include "Base.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "LinkedMap"

namespace pio
{
	/*
	* LinkeMap is a map that manages the items in its insertion order
	*/
	template <class K, class T>
	class LinkedMap
	{
	public:
		typedef typename std::list<T>::iterator ListIterator;
		typedef typename std::list<T>::const_iterator ListCIterator;

		typedef typename std::map<K, T>::iterator MapIterator;

		LinkedMap() : m_list(), m_map() 
		{
		}

		~LinkedMap() 
		{ 
			release(); 
		}

		void add(const K &key, const T &val)
		{
			MapIterator it = m_map.find(key);
			if (it == m_map.end())
			{
				m_list.push_back(val);
				m_map.insert({ key, val });
			}
		}

		T &get(const K &key)
		{
			MapIterator it = m_map.find(key);
			if (it != m_map.end())
			{
				return it->second;
			}
			return T();
		}

		void remove(const K &key)
		{
			MapIterator it = m_map.find(key);
			if (it == m_map.end())
				return;

			T &item = it->second;

			ListIterator listIt = m_list.begin();
			while (listIt != m_list.end())
			{
				if ((*listIt) == item)
				{
					m_list.erase(listIt);
					break;
				}
				listIt++;
			}
			m_map.erase(it);
		}

		bool exist(const K &key) const
		{
			return m_map.find(key) != m_map.end();
		}

		uint32_t size() const
		{
			return m_map.size();
		}

		bool empty() const
		{
			return m_map.empty();
		}

		ListIterator begin()
		{
			return m_list.begin();
		}

		ListIterator end()
		{
			return m_list.end();
		}

		ListCIterator cbegin() const
		{
			return m_list.cbegin();
		}

		ListCIterator cend() const
		{
			return m_list.cend();
		}

		T &operator[](uint32_t index)
		{
			ListIterator it = m_list.begin();
			uint32_t i = 0;
			while (i < index)
			{
				i++;
				it++;
			}
			return (*it);
		}

		void release()
		{
			ListIterator it0 = m_list.begin();
			while (it0 != m_list.end())
			{
				it0 = m_list.erase(it0);
			}

			MapIterator it1 = m_map.begin();
			while (it1 != m_map.end())
			{
				it1 = m_map.erase(it1);
			}
		}

	private:
		std::list<T> m_list{};
		std::map<K, T> m_map{};
	};
}

#endif