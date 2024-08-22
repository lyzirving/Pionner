#ifndef __PIONNER_SCENE_3D_TRIANGLE_MESH_H__
#define __PIONNER_SCENE_3D_TRIANGLE_MESH_H__

#include "Base.h"

namespace pio
{
	struct Vertex3d
	{
		glm::vec3 Pos{ 0.f };
		glm::vec3 Normal{ 0.f };
		glm::vec2 TexCoord{ 0.f };
	};

	struct Triangle
	{
		glm::vec3 Normal{ 0.f };
		uint16_t Indices[3];
	};

	class TriangleMesh
	{
	public:
		TriangleMesh() {}
		~TriangleMesh() = default;

		TriangleMesh(const TriangleMesh& rhs);
		TriangleMesh(TriangleMesh&& rhs) noexcept;

		TriangleMesh& operator=(const TriangleMesh& rhs);
		TriangleMesh& operator=(TriangleMesh&& rhs) noexcept;

		std::vector<Vertex3d> Vertices;
		std::vector<uint16_t> Indices;
		std::vector<Triangle> Triangles;
	};
}

#endif