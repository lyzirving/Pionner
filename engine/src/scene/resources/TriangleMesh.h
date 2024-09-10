#ifndef __PIONNER_SCENE_RESOURCES_TRIANGLE_MESH_H__
#define __PIONNER_SCENE_RESOURCES_TRIANGLE_MESH_H__

#include "Common.h"

namespace pio
{
	template<typename T>
	struct Triangle
	{
		glm::vec3 Normal{ 0.f };
		T Indices[3]{ 0, 0, 0 };

		Triangle() {}

		Triangle(const glm::vec3& n, const T indice[3], int num = 3) : Normal(n)
		{
			for (size_t i = 0; i < std::min(num, 3); i++) { Indices[i] = indice[i]; }
		}

		Triangle(const glm::vec3& n, const std::initializer_list<T> &indice, int num = 3) : Normal(n)
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

	template<typename T, typename U>
	class TriangleMesh
	{
	public:
		TriangleMesh() {}
		~TriangleMesh() = default;

		TriangleMesh(const TriangleMesh<T, U>& rhs)
		{
			Vertices = rhs.Vertices;
			Indices = rhs.Indices;
			Triangles = rhs.Triangles;
		}

		TriangleMesh(TriangleMesh<T, U>&& rhs) noexcept
		{
			Vertices = std::move(rhs.Vertices);
			Indices = std::move(rhs.Indices);
			Triangles = std::move(rhs.Triangles);
		}

		TriangleMesh& operator=(const TriangleMesh<T, U>& rhs)
		{
			if (this != &rhs)
			{
				this->TriangleMesh<T, U>::TriangleMesh(rhs);
			}
			return *this;
		}

		TriangleMesh& operator=(TriangleMesh<T, U>&& rhs) noexcept
		{
			if (this != &rhs)
			{
				this->TriangleMesh<T, U>::TriangleMesh(std::forward<TriangleMesh<T, U>>(rhs));
			}
			return *this;
		}

		std::vector<T> Vertices;
		std::vector<U> Indices;
		std::vector<Triangle<U>> Triangles;
	};

	using TriangleMesh3d = TriangleMesh<Vertex3d, uint16_t>;
}

#endif