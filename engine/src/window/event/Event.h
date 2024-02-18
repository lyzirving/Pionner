#ifndef __PIONNER_WINDOW_EVENT_EVENT_H__
#define __PIONNER_WINDOW_EVENT_EVENT_H__

#include <sstream>

#include "core/Base.h"

namespace pio
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = PIO_BIT(0),
		EventCategoryInput = PIO_BIT(1),
		EventCategoryKeyboard = PIO_BIT(2),
		EventCategoryMouse = PIO_BIT(3),
		EventCategoryMouseButton = PIO_BIT(4)
	};

	// note #type will be turned in to char*
	#define EVENT_CLASS_TYPE(type) static  EventType getStaticType() { return EventType::type; }\
								   virtual EventType getEventType() const override { return getStaticType(); }\
								   virtual const char* getName() const override { return #type; }

	#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

	class Event
	{
	public:
		virtual ~Event() = default;

		virtual EventType getEventType() const = 0;
		virtual const char *getName() const = 0;
		virtual int getCategoryFlags() const = 0;
		virtual std::string toString() const { return getName(); }

		bool isInCategory(EventCategory category) { return getCategoryFlags() & category; }

		template<typename T>
		T *as() { if (is<T>()) { return static_cast<T *>(this); } else { return nullptr; } }

	protected:
		template<typename T>
		bool is() { return false; }

	public:
		bool Handled{ false };
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event &event) : m_event(event)
		{
		}

		// F will be deduced by the compiler
		template<typename T, typename F>
		bool dispatch(const F &func)
		{
			if (m_event.getEventType() == T::getStaticType())
			{
				m_event.Handled |= func(static_cast<T &>(m_event));
				return m_event.Handled;
			}
			return false;
		}
	private:
		// note it's a reference
		Event &m_event;
	};
}

#endif