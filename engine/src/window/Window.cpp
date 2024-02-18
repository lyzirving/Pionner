#include "Window.h"

#include "platform/windows/WindowsWindow.h"

namespace pio
{
	Scope<Window> Window::create(const WindowProps &prop)
	{
	#ifdef PIO_WINDOWS
		return CreateScope<WindowsWindow>(prop);
	#else
		return Scope<Window>(nullptr);
	#endif
	}
}