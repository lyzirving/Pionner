#ifndef __PIONNER_RENDER_PASS_RENDERPASS_H__
#define __PIONNER_RENDER_PASS_RENDERPASS_H__

#include <string>
#include <memory>
#include <vector>

#include "scenegraph/SceneGrfDef.h"

namespace pio
{
	namespace render
	{
		class RenderPass
		{
		public:
			RenderPass() {};
			virtual ~RenderPass() = default;

			virtual void tick(uint64_t deltaTimeMs) = 0;

			virtual void init() {};
			virtual void release() {};
		};
	}
}

#endif	