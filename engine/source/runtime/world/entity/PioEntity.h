#ifndef __PIONNER_PIO_ENTITY_H__
#define __PIONNER_PIO_ENTITY_H__

#include <string>
#include <memory>
#include <decs.h>

#include "world/comp/ComponentFactory.h"

namespace pio
{
	class PioEntity
	{
		friend class PioWorld;
	public:
		PioEntity();
		~PioEntity();

		inline bool compExist(CompDefine d) { return m_comps[d] != nullptr; }
		inline const std::string &getName() { return m_name; }
		inline uint32_t getId() { return m_id; }

		inline void setName(const std::string &name) { m_name = name; }

		template <class ... CompTypes>
		bool createComps();

	private:
		template <class T>
		static bool checkType();

	private:
		uint32_t       m_id;
		decs::EntityID m_ecsId;
		// This filed can be overrided once entity is constructed
		std::string    m_key;
		std::string    m_name;

		std::shared_ptr<Component> m_comps[CMP_CNT];
	};

	template <class T>
	bool PioEntity::checkType()
	{
		return std::is_base_of<Component, T>();
	}

	template <class ... CompTypes>
	bool PioEntity::createComps()
	{
		uint32_t cnt = sizeof...(CompTypes);
		if (cnt == 0)
			return true;

		bool checkTypeResult[] = { checkType<CompTypes>()... };

		for (uint32_t i = 0; i < cnt; i++)
		{
			if (!checkTypeResult[i])
				return false;
		}

		std::shared_ptr<Component> comps[] = { ComponentFactory::create<CompTypes>()... };
		for (uint32_t i = 0; i < cnt; i++)
		{
			m_comps[comps[i]->getDefine()] = comps[i];
		}
		return true;
	}
}

#endif