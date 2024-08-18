#ifndef __PIONNER_SCENE_COMPONENTS_H__
#define __PIONNER_SCENE_COMPONENTS_H__

#include "Base.h"

namespace pio
{
	struct Component
	{
		bool Enable{ true };
	};

	struct CameraComponent : public Component
	{
		UUID32 Handle{ InvalidId };
	};

	struct MeshFilter : public Component
	{

	};

	struct MeshRenderer : public Component
	{

	};
}

#endif