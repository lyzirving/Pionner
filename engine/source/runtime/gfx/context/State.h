#ifndef __PIONNER_GFX_CONTEXT_STATE_H__
#define __PIONNER_GFX_CONTEXT_STATE_H__

#include <memory>
#include <mutex>

#include "attr/Blend.h"
#include "attr/ClearMode.h"
#include "attr/CullFace.h"
#include "attr/DepthTest.h"

namespace pio
{
	namespace gfx
	{
		class State
		{
			friend class GraphicContex;
		public:
			State();
			~State();

			bool init();
			void shutdown();

			void setBlendMode(const Blend &blend);
			void setClearMode(const ClearMode &mode);
			void setCullMode(const CullFace &mode);
			void setDepthMode(const DepthTest &test);

		};
	}
}

#endif