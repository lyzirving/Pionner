#include <iostream>
#include <memory>

#include "Editor.h"
#include "Engine.h"

using namespace pio;

int main()
{
	std::shared_ptr<Engine> engine = std::make_shared<Engine>();
	engine->initialize();

	std::shared_ptr<Editor> editor = std::make_shared<Editor>(engine);
	//TODO: optimize the init-flow of ui
	//editor->initialize();

	editor->run();

	editor->shutdown();
	engine->shutdown();
	return 0;
}
