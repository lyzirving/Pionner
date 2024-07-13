#ifndef __PIONNER_SCENE_SCENE_DEF_H__
#define __PIONNER_SCENE_SCENE_DEF_H__

#include "asset/Asset.h"

namespace pio
{	
	// TODO: support more point light?
	#define POINT_LIGHT_LIMIT 4

	enum class LightType : uint8_t
	{
		None = 0, Directional, Point, Spot, Num
	};

	enum ShadowMode : uint8_t
	{
		ShadowMode_Hard = 0, ShadowMode_Soft, ShadowMode_Soft_2X, ShadowMode_Soft_4X, ShadowMode_Num
	};

	constexpr static const char *ShadowModeNames[ShadowMode_Num] = { "Hard", "Soft", "Soft_2X", "Soft_4X" };

	struct SimpleVertex
	{
		glm::vec3 Position{ 0.f };
		glm::vec2 Texcoord{ 0.f };

		SimpleVertex() {}
		SimpleVertex(const glm::vec3 &pos, const glm::vec2 &tex) : Position(pos), Texcoord(tex) {}
	};

	struct Vertex
	{
		glm::vec3 Position{ 0.f };
		glm::vec3 Normal{ 0.f };
		glm::vec3 Tangent{ 0.f };
		glm::vec3 Binormal{ 0.f };
		glm::vec2 Texcoord{ 0.f };
	};

	struct Index
	{
		uint32_t V0{ 0 }, V1{ 0 }, V2{ 0 };

		Index() {}
		Index(uint32_t v0, uint32_t v1, uint32_t v2) : V0(v0), V1(v1), V2(v2) {}
	};

	struct Triangle
	{
		Vertex V0, V1, V2;

		Triangle(const Vertex &v0, const Vertex &v1, const Vertex &v2)
			: V0(v0), V1(v1), V2(v2) {}
	};

	struct QuadVertex
	{
		glm::vec3 Position{ 0.f };
		glm::vec2 Texcoord{ 0.f };

		QuadVertex() {}
		QuadVertex(const glm::vec2 &pos, const glm::vec2 &texcoord) : Position(glm::vec3(pos.x, pos.y, 0.f)), Texcoord(texcoord) {}
		QuadVertex(const glm::vec3 &pos, const glm::vec2 &texcoord) : Position(pos), Texcoord(texcoord) {}
	};
}

#endif