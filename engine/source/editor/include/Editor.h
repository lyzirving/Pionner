#ifndef __EDITOR_H__
#define __EDITOR_H__

#include <memory>

namespace Pionner
{
	class Engine;

	class Editor
	{
	public:
		Editor(const std::shared_ptr<Engine> &engine);
		virtual ~Editor();

		void initialize();
		void destroy();

		void run();

	private:
		std::shared_ptr<Engine> m_engine_runtime;
	};
}

#endif // !__EDITOR_H__
