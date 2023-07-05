#ifndef __PIONNER_PIO_WORLD_H__
#define __PIONNER_PIO_WORLD_H__

#include <unordered_map>
#include <string.h>
#include <string>
#include <memory>

#include "entity/PioEntity.h"
#include "scenegraph/node/NodeFactory.h"

#include "core/log/LogSystem.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "PioWorld"

namespace pio
{
	namespace scenegrf 
	{
		class Group;
	}

	class PioWorld
	{
	public:
		PioWorld();
		~PioWorld();

		inline bool dirty() const { return m_dirty.load(); }
		inline void setDirty(bool b) { m_dirty.store(b); }
		inline std::shared_ptr<scenegrf::Group> getSceneRoot() { return m_sceneRoot; }

		/**
		* @brief: This method will directly add new node to selected parent node.
		* @param parentNodeName: target node's name, the node should be the parent node of the newly created node
		* @param nodeName:       name of the newly created node
		* @param type:           type of the entity that will hold the newly created node.
		*/
		template <class ... CompTypes>
		std::shared_ptr<PioEntity> addEntity(const std::string &parentNodeName, const std::string &nodeName, PioEntityType type);

		/*
		* @brief: This method will directly add new node to scene root.
		*/
		template <class ... CompTypes>
		std::shared_ptr<PioEntity> addEntity(const std::string &nodeName, PioEntityType type);

		void init();
		void shutdown();

	private:
		template <class ... CompTypes>
		std::shared_ptr<PioEntity> createEntity(PioEntityType type);

	private:
		static uint32_t g_entityId;
		typedef std::unordered_map<std::string, std::shared_ptr<PioEntity>> Collection;

		std::atomic<bool> m_dirty;
		decs::ECSWorld    m_ecsWorld;
		Collection        m_entities;

		std::shared_ptr<scenegrf::Group> m_sceneRoot;
	};

	template<class ...CompTypes>
	std::shared_ptr<PioEntity> PioWorld::addEntity(const std::string &parentNodeName, const std::string &nodeName, 
												   PioEntityType type)
	{
		std::shared_ptr<PioEntity> entity = createEntity<CompTypes...>(type);
		if (entity)
		{
			entity->setName(nodeName);
			m_sceneRoot->addChild(parentNodeName, entity->m_sceneNode);
		}
		else
		{
			LOG_ERR("fail to add entity[type = %u] to node[%s]", type, nodeName.c_str());
		}
		return entity;
	}

	template<class ...CompTypes>
	std::shared_ptr<PioEntity> PioWorld::addEntity(const std::string &nodeName, PioEntityType type)
	{
		std::shared_ptr<PioEntity> entity = createEntity<CompTypes...>(type);
		if (entity)
		{
			entity->setName(nodeName);
			m_sceneRoot->addChild(entity->m_sceneNode);
		}
		else
		{
			LOG_ERR("fail to add entity[type = %u] to node[%s]", type, nodeName.c_str());
		}
		return entity;
	}

	template <class ... CompTypes>
	std::shared_ptr<PioEntity> PioWorld::createEntity(PioEntityType type)
	{
		std::string key = "entity_" + std::to_string(g_entityId);
		auto itr = m_entities.find(key);
		if (itr != m_entities.end())
		{
			LOG_INFO("entity[%s][%s] already exist", key.c_str(), itr->second->m_name.c_str());
			return itr->second;
		}
		auto entity = std::shared_ptr<PioEntity>(new PioEntity(type));
		entity->m_id = g_entityId;
		entity->m_key = key;
		if (entity->createComps<CompTypes ...>())
		{
			entity->m_ecsId = m_ecsWorld.new_entity<CompTypes...>();
			entity->m_sceneNode = scenegrf::NodeFactory::create(type);
			g_entityId++;
			m_entities.insert(std::make_pair(entity->m_key, entity));
			return entity;
		}
		// failed to createComps()
		LOG_ERR("fail to create entity[%s], creating components err!", key.c_str());
		entity.reset();
		return entity;
	}
}

#endif