#ifndef __PIONNER_GFX_PIPELINE_PIPELINE_UTILS_H__
#define __PIONNER_GFX_PIPELINE_PIPELINE_UTILS_H__

#include "gfx/GfxDef.h"

namespace pio
{
	class Entity;
	class Camera;
	class RenderContext;
	struct RenderingData;

	namespace pipeline
	{
		std::vector<Ref<Camera>> FetchCamera(Ref<RenderContext>& context, const std::list<Ref<Entity>>& entities);
		void ProcessMeshEnt(Ref<RenderContext>& context, Ref<Entity>& entity, /*out*/RenderingData& renderingData);
	}
}

#endif