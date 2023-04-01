#include <iostream>

#include "Editor.h"

#include "Engine.h"

namespace Pionner
{
	Editor::Editor(const std::shared_ptr<Engine>& engine) 
		: m_engine_runtime(engine)
	{
	}

	Editor::~Editor() = default;

	void Editor::initialize()
	{
		std::cout << "Editor initialize" << std::endl;
	}

	void Editor::destroy()
	{
	}

	void Editor::run()
	{
	}
}