#ifndef __PIONNER_WINDOW_WINDOW_H__
#define __PIONNER_WINDOW_WINDOW_H__

#include "window/event/Event.h"
#include "window/input/Cursor.h"

namespace pio
{
	struct WindowProps
	{
		std::string m_title;
		uint32_t m_width;
		uint32_t m_height;

		WindowProps(const std::string &title, uint32_t width, uint32_t height)
			: m_title(title), m_width(width), m_height(height)
		{
		}
	};

	class Window
	{
	public:
		using EvtCbFn = std::function<void(Event &)>;

		Window() {};
		virtual ~Window() = default;

	public:
		static Scope<Window> create(const WindowProps &prop);

	public:
		virtual void pollEvents() = 0;
		virtual void setEventCallback(const EvtCbFn &callback) = 0;

		virtual void getWindowPos(int32_t &x, int32_t &y) = 0;
		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		virtual void *getNativeWindow() const = 0;
		virtual glm::vec2 getCursorPos() const = 0;
		virtual void setCursorMode(CursorMode mode) = 0;

		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;		
	};
}

#endif