#ifndef __PIONNER_PIO_WORLD_H__
#define __PIONNER_PIO_WORLD_H__

#include <unordered_map>
#include <string.h>
#include <string>
#include <memory>

#include "PioEntity.h"

#include "scenegraph/view/Scene.h"
#include "scenegraph/node/NodeFactory.h"

#include "core/log/LogSystem.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "PioWorld"

namespace pio
{
	namespace sgf 
	{
		class Group;
	}

	class PioWorld : public std::enable_shared_from_this<PioWorld>
	{
	public:
		PioWorld();
		~PioWorld();

		inline bool dirty() const { return m_dirty.load(); }
		inline void setDirty(bool b) { m_dirty.store(b); }
		inline std::shared_ptr<sgf::Scene> getScene() { return m_scene; }

		/**
		* @brief: This method will directly add new node to selected parent node.
		* @param parentName: target node's name, the node should be the parent node of the newly created node
		* @param nodeName:       name of the newly created node
		* @param type:           type of the entity that will hold the newly created node.
		*/
		template <class ... CompTypes>
		std::shared_ptr<PioEntity> addEntity(const std::string &parentName, const EntityParam &param);

		/*
		* @brief: This method will directly add new node to scene root.
		*/
		template <class ... CompTypes>
		std::shared_ptr<PioEntity> addEntity(const EntityParam &param);

		void attach(const std::shared_ptr<sgf::Scene> &scene);
		void init();
		void swap(uint64_t deltaMs);
		void shutdown();

	private:
		template <class ... CompTypes>
		std::shared_ptr<PioEntity> createEntity(const EntityParam &param);

	private:
		static uint32_t g_entityId;
		typedef std::unordered_map<std::string, std::shared_ptr<PioEntity>> Collection;

		std::atomic<bool> m_dirty{true};
		decs::ECSWorld    m_ecsWorld{};
		Collection        m_entities{};

		std::shared_ptr<sgf::Scene> m_scene;
	};

	template<class ...CompTypes>
	std::shared_ptr<PioEntity> PioWorld::addEntity(const std::string &parentName, const EntityParam &param)
	{
		std::shared_ptr<PioEntity> entity = createEntity<CompTypes...>(param);
		if (entity)
		{
			m_scene->addNode(parentName, entity->m_sceneNode);
		}
		else
		{
			LOG_ERR("fail to add entity[type = %u] to node[%s]", param.type, nodeName.c_str());
		}
		return entity;
	}

	template<class ...CompTypes>
	std::shared_ptr<PioEntity> PioWorld::addEntity(const EntityParam &param)
	{
		std::shared_ptr<PioEntity> entity = createEntity<CompTypes...>(param);
		if (entity)
		{
			m_scene->addNode(entity->m_sceneNode);
		}
		else
		{
			LOG_ERR("fail to add entity[type = %u] to node[%s]", type, nodeName.c_str());
		}
		return entity;
	}

	template <class ... CompTypes>
	std::shared_ptr<PioEntity> PioWorld::createEntity(const EntityParam &param)
	{
		std::string key = "entity_" + std::to_string(g_entityId);
		auto itr = m_entities.find(key);
		if (itr != m_entities.end())
		{
			LOG_INFO("entity[%s][%s] already exist", key.c_str(), itr->second->m_name.c_str());
			return itr->second;
		}
		auto entity = std::shared_ptr<PioEntity>(new PioEntity(param.type, shared_from_this()));
		entity->m_id = g_entityId;
		entity->m_key = key;
		entity->setName(param.nodeName);
		if (entity->createComps<CompTypes ...>())
		{
			entity->m_ecsId = m_ecsWorld.new_entity<CompTypes...>();
			entity->m_sceneNode = sgf::NodeFactory::create(param);
			m_entities.insert(std::make_pair(entity->m_key, entity));
			g_entityId++;
			return entity;
		}
		// failed to createComps()
		LOG_ERR("fail to create entity[%s], creating components err!", key.c_str());
		entity.reset();
		return entity;
	}
}

#endif