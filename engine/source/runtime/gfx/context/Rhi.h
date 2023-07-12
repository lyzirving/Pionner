#ifndef __PIONNER_GFX_CONTEXT_RHI_H__
#define __PIONNER_GFX_CONTEXT_RHI_H__

#include <memory>
#include <mutex>

namespace pio
{

	namespace gfx
	{
		class Rhi
		{
			friend class GraphicContex;
		public:
			Rhi();
			~Rhi();

			bool init();
			void shutdown();

		private:
			bool m_isInit{ false };
		};
	}
}

#endif