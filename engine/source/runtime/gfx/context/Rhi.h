#ifndef __PIONNER_GFX_CONTEXT_RHI_H__
#define __PIONNER_GFX_CONTEXT_RHI_H__

#include <memory>
#include <mutex>

#include "gfx/GfxDef.h"

namespace pio
{
	class WindowSystem;

	namespace gfx
	{
		class Rhi
		{
			friend class GraphicContex;
		public:
			Rhi();
			~Rhi();

			bool init();
			bool initUiBackend(const std::shared_ptr<WindowSystem> &window);
			void shutdown();
			void shutdownUiBackend();

			bool drawTriangleElement(uint32_t indexCnt, DataType type);
			void setViewport(int32_t l, int32_t t, int32_t width, int32_t height);
			void unbindTexture(TextureType type);

		private:
			bool m_isInit{ false };
		};
	}
}

#endif