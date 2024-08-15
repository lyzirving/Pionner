#include "Editor.h"

int main()
{
	auto editor = pio::CreateScope<pio::Editor>();
	editor->run();
	editor.reset(nullptr);
	return 0;
}
