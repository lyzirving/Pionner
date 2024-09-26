#include "Factory.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Factory"

namespace pio
{
	TriangleMesh Factory::MakePlane(float n)
	{
		TriangleMesh triangles;
		glm::vec3 origin(-n / 2.f, 0.f, -n / 2.f);
		uint32_t count = n;
		for(uint32_t row = 0; row < count; row++)
		{
			for(uint32_t col = 0; col < count; col++)
			{
				Vertex3d v0, v1, v2, v3;
				v0.Pos = origin + World::Right * float(col) + World::Forward * float(row);
				v1.Pos = v0.Pos + World::Forward;
				v2.Pos = v1.Pos + World::Right;
				v3.Pos = v2.Pos - World::Forward;

				v0.TexCoord = glm::vec2(float(col) / n, 1.f - float(row) / n);
				v1.TexCoord = glm::vec2(float(col) / n, 1.f - float(row + 1) / n);
				v2.TexCoord = glm::vec2(float(col + 1) / n, 1.f - float(row + 1) / n);
				v3.TexCoord = glm::vec2(float(col + 1) / n, 1.f - float(row) / n);

				v0.Normal = v1.Normal = v2.Normal = v3.Normal = World::Up;				

				MakeTriangleMesh(v0, v1, v2, v3, triangles);
			}
		}
		return triangles;
	}

	TriangleMesh Factory::MakeCube(float n)
	{
		TriangleMesh triangles;
		float half = n * 0.5f;
		//front face
		{
			Vertex3d v0, v1, v2, v3;
			v0.Pos = (World::Forward - World::Right + World::Up) * half;
			v1.Pos = v0.Pos - World::Up * n;
			v2.Pos = v1.Pos + World::Right * n;
			v3.Pos = v0.Pos + World::Right * n;

			v0.TexCoord = glm::vec2(0.f, 1.f);
			v1.TexCoord = glm::vec2(0.f);
			v2.TexCoord = glm::vec2(1.f, 0.f);
			v3.TexCoord = glm::vec2(1.f, 1.f);

			v0.Normal = v1.Normal = v2.Normal = v3.Normal = World::Forward;
			MakeTriangleMesh(v0, v1, v2, v3, triangles);
		}

		//right face
		{
			Vertex3d v0, v1, v2, v3;
			v0.Pos = (World::Right + World::Up + World::Forward) * half;
			v1.Pos = v0.Pos - World::Up * n;
			v2.Pos = v1.Pos - World::Forward * n;
			v3.Pos = v0.Pos - World::Forward * n;

			v0.TexCoord = glm::vec2(0.f, 1.f);
			v1.TexCoord = glm::vec2(0.f);
			v2.TexCoord = glm::vec2(1.f, 0.f);
			v3.TexCoord = glm::vec2(1.f, 1.f);

			v0.Normal = v1.Normal = v2.Normal = v3.Normal = World::Right;
			MakeTriangleMesh(v0, v1, v2, v3, triangles);
		}

		//Left face
		{
			Vertex3d v0, v1, v2, v3;
			v0.Pos = (World::Up - World::Forward - World::Right) * half;
			v1.Pos = v0.Pos - World::Up * n;
			v2.Pos = v1.Pos + World::Forward * n;
			v3.Pos = v0.Pos + World::Forward * n;

			v0.TexCoord = glm::vec2(0.f, 1.f);
			v1.TexCoord = glm::vec2(0.f);
			v2.TexCoord = glm::vec2(1.f, 0.f);
			v3.TexCoord = glm::vec2(1.f, 1.f);

			v0.Normal = v1.Normal = v2.Normal = v3.Normal = -World::Right;
			MakeTriangleMesh(v0, v1, v2, v3, triangles);
		}

		//Back face
		{
			Vertex3d v0, v1, v2, v3;
			v0.Pos = (World::Up - World::Forward + World::Right) * half;
			v1.Pos = v0.Pos - World::Up * n;
			v2.Pos = v1.Pos - World::Right * n;
			v3.Pos = v0.Pos - World::Right * n;

			v0.TexCoord = glm::vec2(0.f, 1.f);
			v1.TexCoord = glm::vec2(0.f);
			v2.TexCoord = glm::vec2(1.f, 0.f);
			v3.TexCoord = glm::vec2(1.f, 1.f);

			v0.Normal = v1.Normal = v2.Normal = v3.Normal = -World::Forward;
			MakeTriangleMesh(v0, v1, v2, v3, triangles);
		}

		//Top face
		{
			Vertex3d v0, v1, v2, v3;
			v0.Pos = (World::Up - World::Forward - World::Right) * half;
			v1.Pos = v0.Pos + World::Forward * n;
			v2.Pos = v1.Pos + World::Right * n;
			v3.Pos = v0.Pos + World::Right * n;

			v0.TexCoord = glm::vec2(0.f, 1.f);
			v1.TexCoord = glm::vec2(0.f);
			v2.TexCoord = glm::vec2(1.f, 0.f);
			v3.TexCoord = glm::vec2(1.f, 1.f);

			v0.Normal = v1.Normal = v2.Normal = v3.Normal = World::Up;
			MakeTriangleMesh(v0, v1, v2, v3, triangles);
		}

		//Bottom face
		{
			Vertex3d v0, v1, v2, v3;
			v0.Pos = (World::Forward - World::Up - World::Right) * half;
			v1.Pos = v0.Pos - World::Forward * n;
			v2.Pos = v1.Pos + World::Right * n;
			v3.Pos = v0.Pos + World::Right * n;

			v0.TexCoord = glm::vec2(0.f, 1.f);
			v1.TexCoord = glm::vec2(0.f);
			v2.TexCoord = glm::vec2(1.f, 0.f);
			v3.TexCoord = glm::vec2(1.f, 1.f);

			v0.Normal = v1.Normal = v2.Normal = v3.Normal = -World::Up;
			MakeTriangleMesh(v0, v1, v2, v3, triangles);
		}
		return triangles;
	}

	TriangleMesh Factory::MakeSquare(float w, float h)
	{
		TriangleMesh triangles;
		glm::vec3 origin(-w / 2.f, h / 2.f, 0.f);
		Vertex3d v0, v1, v2, v3;
		v0.Pos = origin;
		v1.Pos = v0.Pos - World::Up * h;
		v2.Pos = v1.Pos + World::Right * w;
		v3.Pos = v2.Pos + World::Up * h;

		v0.TexCoord = glm::vec2(0.f, 1.f);
		v1.TexCoord = glm::vec2(0.f);
		v2.TexCoord = glm::vec2(1.f, 0.f);
		v3.TexCoord = glm::vec2(1.f, 1.f);

		v0.Normal = v1.Normal = v2.Normal = v3.Normal = World::Forward;

		MakeTriangleMesh(v0, v1, v2, v3, triangles);

		return triangles;
	}

	TriangleMesh Factory::MakeScreenQuad()
	{
		TriangleMesh triangles;
		Vertex3d v0, v1, v2, v3;
		v0.Pos = glm::vec3(-1.f, 1.f, 0.f);
		v1.Pos = glm::vec3(-1.f, -1.f, 0.f);
		v2.Pos = glm::vec3(1.f, -1.f, 0.f);
		v3.Pos = glm::vec3(1.f, 1.f, 0.f);

		v0.TexCoord = glm::vec2(0.f, 1.f);
		v1.TexCoord = glm::vec2(0.f, 0.f);
		v2.TexCoord = glm::vec2(1.f, 0.f);
		v3.TexCoord = glm::vec2(1.f, 1.f);

		v0.Normal = v1.Normal = v2.Normal = v3.Normal = World::Forward;

		MakeTriangleMesh(v0, v1, v2, v3, triangles);

		return triangles;
	}

	void Factory::MakeTriangleMesh(const Vertex3d& v0, const Vertex3d& v1, const Vertex3d& v2, const Vertex3d& v3, TriangleMesh& triangles)
	{
		auto& vertice = triangles.getVertice();
		auto& indice = triangles.getIndice();
		uint16_t idx = vertice.size();

		vertice.push_back(v0);
		vertice.push_back(v1);
		vertice.push_back(v2);
		vertice.push_back(v3);

		indice.push_back(idx);
		indice.push_back(idx + 1);
		indice.push_back(idx + 2);
		indice.push_back(idx + 2);
		indice.push_back(idx + 3);
		indice.push_back(idx);
	}
}