#ifndef __PIONNER_SCENE_COMPONENTS_H__
#define __PIONNER_SCENE_COMPONENTS_H__

#include "Base.h"
#include "gfx/GfxDef.h"

namespace pio
{
	struct Component
	{
		bool Enable{ true };
	};

	struct CameraComponent : public Component
	{
		ProjectionType PrjType{ ProjectionType_Perspective };
		int32_t Depth{ 0 };
		float Fov{ 60.f };
		float Size{ 5.f };
		UUID32 Handle{ InvalidId };
	};

	struct MeshFilter : public Component
	{
		MeshType Type{ MeshType::Num };
		UUID32 Handle{ InvalidId };
	};

	struct MeshRenderer : public Component
	{
		ShaderSpecifier ShaderSpec{ ShaderSpec_Standard };
		RenderingMode Mode{ RenderingMode_Opaque };
	};
}

#endif