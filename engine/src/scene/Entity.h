#ifndef __PIONNER_SCENE_ENTITY_H__
#define __PIONNER_SCENE_ENTITY_H__

#include <decs.h>

#include "core/Base.h"

namespace pio
{
	class Registry;
	class PhysicsActor;

	enum class EntityClass : uint8_t
	{
		None = 0, Scene, Camera,
		MeshSource, Mesh,
		DirectionalLight, PointLight
	};

	class Entity
	{
		friend class Registry;
	public:
		Entity(EntityClass clazz = EntityClass::None);
		Entity(EntityClass clazz, const decs::EntityID& index, const std::string& name);

		Entity(const Entity &rhs);
		Entity(Entity &&rhs) noexcept;

		Entity &operator=(const Entity &rhs);
		Entity &operator=(Entity &&rhs) noexcept;

		bool operator==(const Entity &rhs);
		bool operator!=(const Entity &rhs);

		~Entity() = default;

		bool getGlobalPose(glm::vec3 &out);

		bool setSelection(bool select);
		bool setGlobalPose(const glm::vec3 &translation, const glm::vec3 &euler = glm::vec3(0.f));
		bool setGlobalPoseDiff(const glm::vec3 &diff = glm::vec3(0.f), const glm::vec3 &eulerDiff = glm::vec3(0.f));

		bool setActorGlobalPose(const glm::vec3 &translation, const glm::quat &rotation = quaternion::IDENTITY);

	public:
		void attachActor(const Ref<PhysicsActor> &actor) { m_actor = actor; }
		bool acquireActor(Ref<PhysicsActor> &out) { out = m_actor.lock(); return out.get() != nullptr; }
		bool isClass(EntityClass clazz) const { return m_class == clazz; }

	public:
		template<typename Comp>
		Comp &addComponent()
		{
			s_registry->emplace<Comp>(*this);
			return s_registry->getComponent<Comp>(*this);
		}

		template<typename Comp, typename P, typename...Args>
		Comp &addComponent(P &&p, Args&&...args)
		{
			s_registry->emplace<Comp, P, Args...>(*this, std::forward<P>(p), std::forward<Args>(args)...);
			return s_registry->getComponent<Comp>(*this);
		}

		template<typename Comp>
		bool hasComponent()
		{
			return s_registry->hasComponent<Comp>(*this);
		}

		template<typename Comp>
		bool hasComponent() const
		{
			return s_registry->hasComponent<Comp>(*this);
		}

		template<typename Comp>
		Comp &getComponent()
		{
			return s_registry->getComponent<Comp>(*this);
		}

		template<typename Comp>
		Comp &getComponent() const
		{
			return s_registry->getComponent<Comp>(*this);
		}

		template<typename Comp>
		void removeComponent()
		{
			s_registry->removeComponent<Comp>(*this);
		}

		template<typename ... Comp>
		bool any()
		{
			uint32_t num = sizeof...(Comp);
			if (num == 0)
				return false;

			bool checkRet[] = { s_registry->hasComponent<Comp>(*this)... };
			bool ret = false;
			for (uint32_t i = 0; i < num; i++)
			{
				ret |= checkRet[i];
				if (ret)
					break;
			}
			return ret;
		}

	public:
		const UUID32 &getUid() const { return m_uid; }
		uint32_t getIndex() const { return m_handle.index; }
		EntityClass getClass() const { return m_class; }
		std::string_view getClassName() const { return magic_enum::enum_name(m_class); }
		const std::string &getName() const { return m_name; }

		void setClass(EntityClass clazz) { m_class = clazz; }
		void setName(const std::string& name) { m_name = name; }

	private:
		static Registry *s_registry;

	private:
		UUID32 m_uid;
		decs::EntityID m_handle;
		EntityClass m_class{ EntityClass::None };
		std::string m_name{};
		WeakRef<PhysicsActor> m_actor;
	};

	class EntityView
	{
	public:
		EntityView() {}

		~EntityView()
		{
			auto it = m_entities.begin();
			while (it != m_entities.end())
			{
				it->second.reset();
				it = m_entities.erase(it);
			}
		}

		void add(const Ref<Entity> &entity) { if (entity) { m_entities[entity->getUid()] = entity; } }
		bool empty() { return m_entities.empty(); }
		void remove(const UUID32 &id) { m_entities.erase(id); }
		uint32_t size() const { return m_entities.size(); }

		std::unordered_map<UUID32, Ref<Entity>>::iterator begin() { return m_entities.begin(); }
		std::unordered_map<UUID32, Ref<Entity>>::iterator end() { return m_entities.end(); }

		std::unordered_map<UUID32, Ref<Entity>>::const_iterator begin() const { return m_entities.begin(); }
		std::unordered_map<UUID32, Ref<Entity>>::const_iterator end() const { return m_entities.end(); }

	private:
		std::unordered_map<UUID32, Ref<Entity>> m_entities{};
	};
}

#endif