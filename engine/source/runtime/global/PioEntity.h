#ifndef __PIONNER_PIO_ENTITY_H__
#define __PIONNER_PIO_ENTITY_H__

#include <string>
#include <memory>
#include <atomic>
#include <decs.h>

#include "global/comp/ComponentFactory.h"
#include "GlobalDef.h"

#include "scenegraph/node/Node.h"

namespace pio
{
	class PioWorld;

	class PioEntity
	{
		friend class PioWorld;
	public:
		PioEntity(PioEntityType type, const std::shared_ptr<PioWorld> &world);
		~PioEntity();

		inline bool compExist(CompDefine d) { return m_comps[d] != nullptr; }
		inline const std::string &getName() { return m_name; }
		inline uint32_t getId() { return m_id; }
		inline bool dirty() { return m_dirty.load(); }
		inline void setDirty(bool b) { m_dirty.store(b); }

		void requestUpdate();
		void setName(const std::string &name);
		void swap(uint64_t deltaMs);

		template <class ... CompTypes>
		bool createComps();

	private:
		template <class T>
		static bool checkType();

	private:
		uint32_t                   m_id{0};
		decs::EntityID             m_ecsId{};
		std::shared_ptr<PioWorld>  m_world{nullptr};
		// This filed can not be overrided once entity is constructed.
		std::string                m_key{};
		// Name of the entity, pay attention that m_name should be the same with m_sceneNode's m_name.
		std::string                m_name{};
		// Resource's path if the entity has any resources.
		std::string                m_path{};
		PioEntityType              m_type{ PIO_ENTITY_CNT };
		std::atomic<bool>          m_dirty{true};
		std::shared_ptr<sgf::Node> m_sceneNode{nullptr};
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