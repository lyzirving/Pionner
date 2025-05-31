#pragma once

#include "base/TypeDef.h"

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

		LinkedMap() : m_List(), m_Map() {}
		~LinkedMap()  { Release(); }

		void Add(const K &key, const T &val)
		{
			MapIterator it = m_Map.find(key);
			if (it == m_Map.end())
			{
				m_List.push_back(val);
				m_Map.insert({ key, val });
			}
		}

		T &Get(const K &key)
		{
			MapIterator it = m_Map.find(key);
			if (it != m_Map.end())
			{
				return it->second;
			}
			return T();
		}

		void Remove(const K &key)
		{
			MapIterator it = m_Map.find(key);
			if (it == m_Map.end())
				return;

			T &item = it->second;

			ListIterator listIt = m_List.begin();
			while (listIt != m_List.end())
			{
				if ((*listIt) == item)
				{
					m_List.erase(listIt);
					break;
				}
				listIt++;
			}
			m_Map.erase(it);
		}

		bool Exist(const K &key) const { return m_Map.find(key) != m_Map.end(); }
		uint32_t Size() const { return m_Map.size(); }
		bool Empty() const { return m_Map.empty(); }
		ListIterator Begin() { return m_List.begin(); }
		ListIterator End() { return m_List.end(); }
		ListCIterator CBegin() const { return m_List.cbegin(); }
		ListCIterator CEnd() const { return m_List.cend(); }
		const std::list<T>& Items() const { return m_List; }

		T &operator[](uint32_t index)
		{
			ListIterator it = m_List.begin();
			uint32_t i = 0;
			while (i < index)
			{
				i++;
				it++;
			}
			return (*it);
		}

		void Release()
		{
			ListIterator it0 = m_List.begin();
			while (it0 != m_List.end())
			{
				it0 = m_List.erase(it0);
			}

			MapIterator it1 = m_Map.begin();
			while (it1 != m_Map.end())
			{
				it1 = m_Map.erase(it1);
			}
		}		

	private:
		std::list<T> m_List{};
		std::map<K, T> m_Map{};
	};
}