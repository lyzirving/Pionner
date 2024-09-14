#ifndef __PIONNER_GFX_PIPELINE_PIPELINE_UTILS_H__
#define __PIONNER_GFX_PIPELINE_PIPELINE_UTILS_H__

#include "gfx/GfxDef.h"

namespace pio
{
	class Entity;
	class Camera;
	class RenderContext;
	struct RenderingData;
	struct CameraComponent;

	namespace Pipeline
	{
		void UpdateCamera(Ref<RenderContext>& context, Ref<Entity>& entity, Ref<Camera>& camera);
		void ProcessMeshEnt(Ref<RenderContext>& context, Ref<Entity>& entity, /*out*/RenderingData& renderingData);
	}
}

#endif