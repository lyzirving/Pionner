#include "Window.h"

#include "platform/windows/WindowsWindow.h"

namespace pio
{
	Ref<Window> Window::create(const WindowProps &prop)
	{
	#ifdef PIO_WINDOWS
		return CreateRef<WindowsWindow>(prop);
	#else
		return Scope<Window>(nullptr);
	#endif
	}
}