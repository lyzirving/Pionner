#ifndef __PIONNER_GFX_GRAPHIC_CONTEXT_H__
#define __PIONNER_GFX_GRAPHIC_CONTEXT_H__

#include <memory>

#include "State.h"
#include "Rhi.h"

namespace pio
{
	namespace gfx
	{
		class GraphicContext
		{
		public:
			GraphicContext();
			~GraphicContext();

			bool init();
			void shutdown();

			inline const std::shared_ptr<State> &getState() { return m_state; }
			inline const std::shared_ptr<Rhi>   &getRhi()   { return m_rhi; }

		private:
			std::shared_ptr<State> m_state{ nullptr };
			std::shared_ptr<Rhi>   m_rhi{ nullptr };
		};
	}
}

#endif