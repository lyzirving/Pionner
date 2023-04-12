#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <string>
#include <atomic>
#include <memory>

#include "function/framework/components/ComponentDef.h"

namespace Pionner
{
	class Component;
	class EventSlot;

	class GameObject
	{
	public:
		GameObject(const char* url = nullptr);
		virtual ~GameObject();

		inline uint32_t getId() const { return m_id; }
		inline const std::string getName() const { return m_name; }
		inline bool compTypeValid(ComponentType type) { return (type > CMP_NONE && type < CMP_COUNT); }

		bool addComponent(const std::shared_ptr<Component> &cmp);
		std::shared_ptr<Component> getComponent(ComponentType type);
		bool hasComponent(ComponentType type);

		void tick(float delta);
		void tickLogicEvent(const std::shared_ptr<EventSlot> &slot);

	protected:
		static std::atomic<uint32_t> g_objectId;

		uint32_t m_id;
		std::string m_name;
		std::string m_resUrl;

		std::shared_ptr<Component> m_components[CMP_COUNT];
	};
}

#endif