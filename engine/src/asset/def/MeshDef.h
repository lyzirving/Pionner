#pragma once

#include "Common.h"

namespace pio
{
	namespace MeshAsset
	{		
		constexpr const char* CUBE = "cube";
		constexpr const char* PLANE = "plane";
		constexpr const char* SPHERE = "sphere";
		constexpr const char* DIRLIT_GIZMO = "dirlit-gizmo";
		constexpr const char* POINTLIT_GIZMO = "pointlit-gizmo";
	};

	struct Vertex
	{
		glm::vec3 Pos{ 0.f };
		glm::vec2 TexCoord{ 0.f };
		glm::vec3 Normal{ 0.f };
		glm::vec3 Tangent{ 0.f };
		glm::vec3 Bitangent{ 0.f };

		Vertex() {}
		Vertex(const glm::vec3& pos) : Pos(pos) {}
		Vertex(const glm::vec3& pos, const glm::vec2& texCoord) : Pos(pos), TexCoord(texCoord) {}
		Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& n) : Pos(pos), TexCoord(texCoord), Normal(n) {}
		Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& n, const glm::vec3& tan) : Pos(pos), TexCoord(texCoord), Normal(n), Tangent(tan) {}
		Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& n, const glm::vec3& tan, const glm::vec3& bitan) : Pos(pos), TexCoord(texCoord), Normal(n), Tangent(tan), Bitangent(bitan) {}
	};

	struct SubMesh
	{
		std::string Name{};
		std::string FullPath{};

		uint32_t VerticeOffset{ 0 };
		uint32_t IndexOffset{ 0 };
		uint32_t VerticeNum{ 0 };
		uint32_t IndexNum{ 0 };
		uint32_t MaterialIdx{ 65535 };

		std::vector<Vertex> Vertice;
		std::vector<uint16_t> Indice;

		AABB BoundingBox;

		glm::mat4 Transform{ 1.f };//transform relative to root
		glm::mat4 LocalTransform{ 1.f };//transform relative to parent
	};

	struct SubMeshNode
	{
		std::vector<uint32_t> SubMeshes;//index of mesh belonged to this node
		std::vector<SubMeshNode> Children;
		std::string Name{ "Unknown" };
		glm::mat4 LocalTransform{ 1.f };//transform relative to parent
		uint32_t Level{ 0 };
	};
}