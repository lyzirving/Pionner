#include "Editor.h"

int main()
{
	auto editor = pio::CreateScope<pio::Editor>();
	editor->Run();
	editor.reset(nullptr);
	return 0;
}
