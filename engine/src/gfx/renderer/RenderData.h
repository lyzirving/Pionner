#ifndef __PIONNER_GFX_RENDERER_RENDER_DATA_H__
#define __PIONNER_GFX_RENDERER_RENDER_DATA_H__

#include "asset/Asset.h"
#include "RenderState.h"

namespace pio
{
	class MaterialTable;

	namespace SceneDataAttr
	{
		static const char *U_FrustumNear = "u_frustumNear";
		static const char *U_FrustumFar = "u_frustumFar";
		static const char *U_PointLightPosition = "u_pointLightPos";
		static const char *U_PointLightMat = "u_pointLightMat";
	}

	// NOTE: definition order should not be changed
	enum LightDir : uint8_t
	{
		LightDir_PositiveX = 0,  // Right
		LightDir_NegativeX,      // Left
		LightDir_PositiveY,      // Top
		LightDir_NegativeY,      // Bottom
		LightDir_NegativeZ,      // Back.  In OpenGL, it is marked as GL_TEXTURE_CUBE_MAP_POSITIVE_Z	
		LightDir_PositiveZ,      // Front. In OpenGL, it is marked as GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		LightDir_Num
	};

	static const char *LightDirStr(LightDir dir)
	{
		switch (dir)
		{
			case LightDir_PositiveX:
				return "Right";
			case LightDir_NegativeX:
				return "Left";
			case LightDir_PositiveY:
				return "Top";
			case LightDir_NegativeY:
				return "Bottom";
			case LightDir_NegativeZ:
				return "Back";
			case LightDir_PositiveZ:
				return "Front";
			default:
				return "Invalid Light Direction";
		}
	}

	struct MeshKey
	{
		AssetHandle MeshHandle{ 0 };
		uint32_t SubmeshIndex{ 0 };

		bool operator<(const MeshKey &other) const
		{
			if (MeshHandle < other.MeshHandle)
				return true;

			if (MeshHandle > other.MeshHandle)
				return false;

			return SubmeshIndex <= other.SubmeshIndex;
		}
	};

	struct DrawCommand
	{
		AssetHandle Mesh{ NullAsset };
		uint32_t SubmeshIndex{ NullAsset };
		Ref<MaterialTable> MaterialTb;
		glm::mat4 ModelMat{ 1.f };
		RenderState State{};
		bool IsRigged{ false };
	};

	struct SpriteCommand
	{
		AssetHandle QuadMesh{ NullAsset };
		AssetHandle Texture{ NullAsset };
		RenderState State{};
	};
}

#endif