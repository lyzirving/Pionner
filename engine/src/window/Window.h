#ifndef __PIONNER_WINDOW_WINDOW_H__
#define __PIONNER_WINDOW_WINDOW_H__

#include "event/Event.h"
#include "event/Cursor.h"

namespace pio
{
	class RenderContext;

	struct WindowProps
	{
		std::string Title{};
		uint32_t Width{ 0 };
		uint32_t Height{ 0 };
		BackendFlags Backend{ Backend_Num };

		WindowProps() {}

		WindowProps(const std::string& title, uint32_t width, uint32_t height, BackendFlags type = Backend_OpenGL)
			: Title(title), Width(width), Height(height), Backend(type)
		{
		}
	};

	class Window
	{
	public:
		Window() {};
		virtual ~Window() = default;

	public:
		static Ref<Window> create(const WindowProps &prop);

	public:
		virtual bool init() = 0;
		virtual void shutdown() = 0;

		virtual void makeCurrent() = 0;
		virtual void swapBuffer() = 0;

		virtual void pollEvents() = 0;

		virtual glm::ivec2 position() const = 0;
		virtual glm::vec2 cursor() const = 0;
		virtual uint32_t width() const = 0;
		virtual uint32_t height() const = 0;
		virtual void *nativeWindow() const = 0;
		
		virtual void setCursorMode(CursorMode mode) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;		
	};
}

#endif