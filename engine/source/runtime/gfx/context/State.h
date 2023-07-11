#ifndef __PIONNER_GFX_CONTEXT_STATE_H__
#define __PIONNER_GFX_CONTEXT_STATE_H__

#include <memory>
#include <mutex>

namespace pio
{
	namespace gfx
	{
		class State
		{
		public:
			static State *self();

		private:
			static State      *g_instance;
			static std::mutex  g_objMutex;

			State();
			~State();
		};
	}
}

#endif