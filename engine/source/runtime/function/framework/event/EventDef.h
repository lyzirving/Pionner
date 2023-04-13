#ifndef __EVENT_DEF_H__
#define __EVENT_DEF_H__

#include <memory>

namespace Pionner
{
	class Component;

	enum EventType : uint8_t
	{
		EVT_INVALID,
		EVT_MESH_ADD,
		EVT_COUNT
	};

	class EventArg
	{
	public:
		EventArg() {}
		virtual ~EventArg() = default;
	};

	class EventSlot
	{
	public:
		EventSlot() : m_comp(nullptr), m_arg(nullptr), m_type(EVT_INVALID) {}
		EventSlot(EventType type, const std::shared_ptr<Component> &comp, const std::shared_ptr<EventArg> &arg) 
			: m_comp(comp), m_arg(arg), m_type(type) {}

		~EventSlot()
		{
			m_comp.reset();
			m_arg.reset();
		}

		std::shared_ptr<Component> m_comp;
		std::shared_ptr<EventArg>  m_arg;
		EventType                  m_type;
	};
}

#endif