#pragma once

#include <sstream>

#include "Common.h"

namespace pio
{
	#define	EVENT_ID_ON_NODE_SELECT 0x01

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize,
		MouseScrolled,
		Custom
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = PIO_BIT_MOV(0),
		EventCategoryInput = PIO_BIT_MOV(1),
		EventCategoryKeyboard = PIO_BIT_MOV(2),
		EventCategoryMouse = PIO_BIT_MOV(3),
		EventCategoryMouseButton = PIO_BIT_MOV(4)
	};

	// note #type will be turned in to char*
	#define EVENT_CLASS_TYPE(type) static  EventType StaticType() { return EventType::type; }\
								   virtual EventType GetEventType() const override { return StaticType(); }\
								   virtual const char* GetName() const override { return #type; }

	#define EVENT_CLASS_CATEGORY(category) virtual int CategoryFlags() const override { return category; }

	class Event : public Object
	{
		RTTR_ENABLE(Object)
	public:
		Event() {}
		Event(uint32_t id) : Id(id) {}
		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;
		virtual const char *GetName() const = 0;
		virtual int CategoryFlags() const = 0;
		virtual std::string ToString() const override { return GetName(); }

	public:
		bool Handled{ false };
		uint32_t Id{ 0 };
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(const Ref<Event> &event) : m_Event(event) {}

		// F will be deduced by the compiler
		template<typename T, typename F>
		bool Dispatch(const F &func)
		{
			if(m_Event && m_Event->GetEventType() == T::StaticType())
			{
				m_Event->Handled |= func(RefCast<Event, T>(m_Event));
				return m_Event->Handled;
			}
			return false;
		}
	private:		
		Ref<Event> m_Event;
	};
}