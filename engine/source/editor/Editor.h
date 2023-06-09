#ifndef __EDITOR_H__
#define __EDITOR_H__

#include <memory>

namespace pio
{
	class Engine;
	class EditorUI;

	class Editor
	{
	public:
		Editor(const std::shared_ptr<Engine> &engine);
		virtual ~Editor();

		void initialize();
		void shutdown();

		void run();

	private:
		std::shared_ptr<Engine>   m_engine;
		std::shared_ptr<EditorUI> m_ui;
	};
}

#endif // !__EDITOR_H__
