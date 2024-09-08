#ifndef __PIONNER_SCENE_RESOURCES_TRIANGLE_MESH_H__
#define __PIONNER_SCENE_RESOURCES_TRIANGLE_MESH_H__

#include "Entry.h"

namespace pio
{
	struct Triangle
	{
		glm::vec3 Normal{ 0.f };
		uint16_t Indices[3]{ 0, 0, 0 };

		Triangle() {}
		Triangle(const glm::vec3& n, const int indice[3], int num = 3) : Normal(n)
		{
			for (size_t i = 0; i < std::min(num, 3); i++) { Indices[i] = indice[i]; }
		}
		Triangle(const glm::vec3& n, const std::initializer_list<uint16_t> &indice, int num = 3) : Normal(n)
		{
			int cnt = std::min(3, num);
			int i{ 0 };
			for (auto &idx : indice)
			{
				if (i >= cnt) { break; }
				Indices[i] = idx;
				i++;
			}
		}
	};

	class TriangleMesh
	{
	public:
		TriangleMesh() {}
		~TriangleMesh() = default;

		TriangleMesh(const TriangleMesh& rhs)
		{
			Vertices = rhs.Vertices;
			Indices = rhs.Indices;
			Triangles = rhs.Triangles;
		}

		TriangleMesh(TriangleMesh&& rhs) noexcept
		{
			Vertices = std::move(rhs.Vertices);
			Indices = std::move(rhs.Indices);
			Triangles = std::move(rhs.Triangles);
		}

		TriangleMesh& operator=(const TriangleMesh& rhs)
		{
			if (this != &rhs)
			{
				this->TriangleMesh::TriangleMesh(rhs);
			}
			return *this;
		}

		TriangleMesh& operator=(TriangleMesh&& rhs) noexcept
		{
			if (this != &rhs)
			{
				this->TriangleMesh::TriangleMesh(std::forward<TriangleMesh>(rhs));
			}
			return *this;
		}

		std::vector<Vertex3d> Vertices;
		std::vector<uint16_t> Indices;
		std::vector<Triangle> Triangles;
	};
}

#endif