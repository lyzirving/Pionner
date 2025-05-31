#pragma once

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
		RenderBackendFlags Backend{ RenderBackend_Num };

		WindowProps() {}

		WindowProps(const std::string& title, uint32_t width, uint32_t height, RenderBackendFlags type = RenderBackend_OpenGL)
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
		static Ref<Window> Create(const WindowProps &prop);

	public:
		virtual bool Init() = 0;
		virtual void Shutdown() = 0;

		virtual void MakeCurrent() = 0;
		virtual void SwapBuffer() = 0;

		virtual void PollEvents() = 0;

		virtual glm::ivec2 Position() const = 0;
		virtual glm::vec2 Cursor() const = 0;
		virtual uint32_t Width() const = 0;
		virtual uint32_t Height() const = 0;
		virtual void *NativeWindow() const = 0;
		
		virtual void SetCursorMode(CursorMode mode) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;		
	};
}