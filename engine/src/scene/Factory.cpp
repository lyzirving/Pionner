#include "Factory.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Factory"

namespace pio
{
	Ref<TriangleMesh> Factory::MakePlane(float w, float h)
	{
		auto triangles = CreateRef<TriangleMesh>();
		glm::vec3 origin(-w / 2.f, 0.f, -h / 2.f);
		uint32_t rowNum = h;
		uint32_t colNum = w;
		for(uint32_t row = 0; row < rowNum; row++)
		{
			for(uint32_t col = 0; col < colNum; col++)
			{
				Vertex3d v0, v1, v2, v3;
				v0.Pos = origin + World::Right * float(col) + World::Forward * float(row);
				v1.Pos = v0.Pos + World::Forward;
				v2.Pos = v1.Pos + World::Right;
				v3.Pos = v2.Pos - World::Forward;

				v0.TexCoord = glm::vec2(float(col) / w, 1.f - float(row) / h);
				v1.TexCoord = glm::vec2(float(col) / w, 1.f - float(row + 1) / h);
				v2.TexCoord = glm::vec2(float(col + 1) / w, 1.f - float(row + 1) / h);
				v3.TexCoord = glm::vec2(float(col + 1) / w, 1.f - float(row) / h);

				v0.Normal = v1.Normal = v2.Normal = v3.Normal = World::Up;				

				MakeTriangleMesh(v0, v1, v2, v3, triangles);
			}
		}
		return triangles;
	}

	Ref<TriangleMesh> Factory::MakeCube(float n)
	{
		auto triangles = CreateRef<TriangleMesh>();
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

	Ref<TriangleMesh> Factory::MakeSquare(float w, float h)
	{
		auto triangles = CreateRef<TriangleMesh>();
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

	Ref<LineSeqMesh> Factory::MakeDirLightGizmo(float r, float len)
	{
		auto lineSeqs = CreateRef<LineSeqMesh>();

		const size_t itr = 8;
		const float span = 360.f / float(itr);
		float angle{ 0.f };

		auto& vertice = lineSeqs->getVertice();
		auto& indice = lineSeqs->getIndice();

		vertice.reserve(17);
		vertice.emplace_back(glm::vec3(0.f));

		for (size_t i = 0; i < itr; i++)
		{
			angle = glm::radians(i * span);
			vertice.emplace_back(glm::vec3(r * std::cos(angle), r * std::sin(angle), 0.f));
			vertice.emplace_back(glm::vec3(r * std::cos(angle), r * std::sin(angle), len));
		}

		for (size_t i = 0; i < itr; i++)
		{
			// radial edge
			indice.push_back(0);
			indice.push_back(2 * i + 1);

			// subtense
			if (i == itr - 1)
			{
				indice.push_back(2 * i + 1);
				indice.push_back(1);
			}
			else
			{
				indice.push_back(2 * i + 1);
				indice.push_back(2 * i + 3);
			}

			// light edge
			indice.push_back(2 * i + 1);
			indice.push_back(2 * i + 2);
		}
		return lineSeqs;
	}

	Ref<TriangleMesh> Factory::MakeScreenQuad()
	{
		auto triangles = CreateRef<TriangleMesh>();
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

	void Factory::MakeTriangleMesh(const Vertex3d& v0, const Vertex3d& v1, const Vertex3d& v2, const Vertex3d& v3, Ref<TriangleMesh>& triangles)
	{
		auto& vertice = triangles->getVertice();
		auto& indice = triangles->getIndice();
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