#include "MeshFactory.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "MeshFactory"

namespace pio
{
	SubMesh MeshFactory::MakePlane(float w, float h)
	{
		SubMesh subMesh;
		glm::vec3 origin(-w / 2.f, 0.f, -h / 2.f);
		uint32_t rowNum = h;
		uint32_t colNum = w; 
		for(uint32_t row = 0; row < rowNum; row++)
		{
			for(uint32_t col = 0; col < colNum; col++)
			{
				Vertex v0, v1, v2, v3;
				v0.Pos = origin + World::Right * float(col) + World::Forward * float(row);
				v1.Pos = v0.Pos + World::Forward;
				v2.Pos = v1.Pos + World::Right;
				v3.Pos = v2.Pos - World::Forward;

				subMesh.BoundingBox.Update(v0.Pos);
				subMesh.BoundingBox.Update(v1.Pos);
				subMesh.BoundingBox.Update(v2.Pos);
				subMesh.BoundingBox.Update(v3.Pos);

				v0.TexCoord = glm::vec2(float(col) / w, 1.f - float(row) / h);
				v1.TexCoord = glm::vec2(float(col) / w, 1.f - float(row + 1) / h);
				v2.TexCoord = glm::vec2(float(col + 1) / w, 1.f - float(row + 1) / h);
				v3.TexCoord = glm::vec2(float(col + 1) / w, 1.f - float(row) / h);

				v0.Normal = v1.Normal = v2.Normal = v3.Normal = World::Up;

				MakeVertexMesh(v0, v1, v2, v3, subMesh);
			}
		}
		auto& vertice = subMesh.Vertice;
		auto& indice = subMesh.Indice;
		for(size_t i = 0; i < indice.size(); i += 3)
		{
			auto& v0 = vertice[indice[i]];
			auto& v1 = vertice[indice[i + 1]];
			auto& v2 = vertice[indice[i + 2]];
			CalcTangent(v0, v1, v2);
		}
		return subMesh;
	}

	SubMesh MeshFactory::MakeCube(float n)
	{
		SubMesh subMesh;
		float half = n * 0.5f;
		//front face
		{
			Vertex v0, v1, v2, v3;
			v0.Pos = (World::Forward - World::Right + World::Up) * half;
			v1.Pos = v0.Pos - World::Up * n;
			v2.Pos = v1.Pos + World::Right * n;
			v3.Pos = v0.Pos + World::Right * n;

			subMesh.BoundingBox.Update(v0.Pos);
			subMesh.BoundingBox.Update(v1.Pos);
			subMesh.BoundingBox.Update(v2.Pos);
			subMesh.BoundingBox.Update(v3.Pos);

			v0.TexCoord = glm::vec2(0.f, 1.f);
			v1.TexCoord = glm::vec2(0.f);
			v2.TexCoord = glm::vec2(1.f, 0.f);
			v3.TexCoord = glm::vec2(1.f, 1.f);

			v0.Normal = v1.Normal = v2.Normal = v3.Normal = World::Forward;
			MakeVertexMesh(v0, v1, v2, v3, subMesh);
		}

		//right face
		{
			Vertex v0, v1, v2, v3;
			v0.Pos = (World::Right + World::Up + World::Forward) * half;
			v1.Pos = v0.Pos - World::Up * n;
			v2.Pos = v1.Pos - World::Forward * n;
			v3.Pos = v0.Pos - World::Forward * n;

			subMesh.BoundingBox.Update(v0.Pos);
			subMesh.BoundingBox.Update(v1.Pos);
			subMesh.BoundingBox.Update(v2.Pos);
			subMesh.BoundingBox.Update(v3.Pos);

			v0.TexCoord = glm::vec2(0.f, 1.f);
			v1.TexCoord = glm::vec2(0.f);
			v2.TexCoord = glm::vec2(1.f, 0.f);
			v3.TexCoord = glm::vec2(1.f, 1.f);

			v0.Normal = v1.Normal = v2.Normal = v3.Normal = World::Right;
			MakeVertexMesh(v0, v1, v2, v3, subMesh);
		}

		//Left face
		{
			Vertex v0, v1, v2, v3;
			v0.Pos = (World::Up - World::Forward - World::Right) * half;
			v1.Pos = v0.Pos - World::Up * n;
			v2.Pos = v1.Pos + World::Forward * n;
			v3.Pos = v0.Pos + World::Forward * n;

			subMesh.BoundingBox.Update(v0.Pos);
			subMesh.BoundingBox.Update(v1.Pos);
			subMesh.BoundingBox.Update(v2.Pos);
			subMesh.BoundingBox.Update(v3.Pos);

			v0.TexCoord = glm::vec2(0.f, 1.f);
			v1.TexCoord = glm::vec2(0.f);
			v2.TexCoord = glm::vec2(1.f, 0.f);
			v3.TexCoord = glm::vec2(1.f, 1.f);

			v0.Normal = v1.Normal = v2.Normal = v3.Normal = -World::Right;
			MakeVertexMesh(v0, v1, v2, v3, subMesh);
		}

		//Back face
		{
			Vertex v0, v1, v2, v3;
			v0.Pos = (World::Up - World::Forward + World::Right) * half;
			v1.Pos = v0.Pos - World::Up * n;
			v2.Pos = v1.Pos - World::Right * n;
			v3.Pos = v0.Pos - World::Right * n;

			subMesh.BoundingBox.Update(v0.Pos);
			subMesh.BoundingBox.Update(v1.Pos);
			subMesh.BoundingBox.Update(v2.Pos);
			subMesh.BoundingBox.Update(v3.Pos);

			v0.TexCoord = glm::vec2(0.f, 1.f);
			v1.TexCoord = glm::vec2(0.f);
			v2.TexCoord = glm::vec2(1.f, 0.f);
			v3.TexCoord = glm::vec2(1.f, 1.f);

			v0.Normal = v1.Normal = v2.Normal = v3.Normal = -World::Forward;
			MakeVertexMesh(v0, v1, v2, v3, subMesh);
		}

		//Top face
		{
			Vertex v0, v1, v2, v3;
			v0.Pos = (World::Up - World::Forward - World::Right) * half;
			v1.Pos = v0.Pos + World::Forward * n;
			v2.Pos = v1.Pos + World::Right * n;
			v3.Pos = v0.Pos + World::Right * n;

			subMesh.BoundingBox.Update(v0.Pos);
			subMesh.BoundingBox.Update(v1.Pos);
			subMesh.BoundingBox.Update(v2.Pos);
			subMesh.BoundingBox.Update(v3.Pos);

			v0.TexCoord = glm::vec2(0.f, 1.f);
			v1.TexCoord = glm::vec2(0.f);
			v2.TexCoord = glm::vec2(1.f, 0.f);
			v3.TexCoord = glm::vec2(1.f, 1.f);

			v0.Normal = v1.Normal = v2.Normal = v3.Normal = World::Up;
			MakeVertexMesh(v0, v1, v2, v3, subMesh);
		}

		//Bottom face
		{
			Vertex v0, v1, v2, v3;
			v0.Pos = (World::Forward - World::Up - World::Right) * half;
			v1.Pos = v0.Pos - World::Forward * n;
			v2.Pos = v1.Pos + World::Right * n;
			v3.Pos = v0.Pos + World::Right * n;

			subMesh.BoundingBox.Update(v0.Pos);
			subMesh.BoundingBox.Update(v1.Pos);
			subMesh.BoundingBox.Update(v2.Pos);
			subMesh.BoundingBox.Update(v3.Pos);

			v0.TexCoord = glm::vec2(0.f, 1.f);
			v1.TexCoord = glm::vec2(0.f);
			v2.TexCoord = glm::vec2(1.f, 0.f);
			v3.TexCoord = glm::vec2(1.f, 1.f);

			v0.Normal = v1.Normal = v2.Normal = v3.Normal = -World::Up;
			MakeVertexMesh(v0, v1, v2, v3, subMesh);
		}

		auto& vertice = subMesh.Vertice;
		auto& indice = subMesh.Indice;
		for (size_t i = 0; i < indice.size(); i += 3)
		{
			auto& v0 = vertice[indice[i]];
			auto& v1 = vertice[indice[i + 1]];
			auto& v2 = vertice[indice[i + 2]];
			CalcTangent(v0, v1, v2);
		}

		return subMesh;
	}

	SubMesh MeshFactory::MakeSphere(float radius, int32_t itr)
	{
		SubMesh subMesh;
		auto& vertice = subMesh.Vertice;
		auto& indice = subMesh.Indice;

		const int32_t horItr = itr;
		const int32_t verItr = itr;
		const int32_t total = 2 + (verItr - 1) * horItr;

		auto GetVertexInd = [horItr, verItr, total](int32_t pX, int32_t pY)
		{
			if (pX == 0 && pY == 0)
				return 0;
			else if (pX == 0 && pY == verItr)
				return total - 1;
			else if (pY != 0 && pY != verItr)
				return (1 + (pY - 1) * horItr + pX);

			return 0;
		};

		for (int32_t y = 0; y <= verItr; y++)
		{
			for (int32_t x = 0; x < horItr; x++)
			{
				if ((y == 0 && x == 0) || (y != 0 && y != verItr) || (y == verItr && x == 0))
				{
					Vertex v{};
					// theta is the angle between vector projected on x-z plane and +z
					// phi is the angle between vector and +y
					float theta = float(x) / float(horItr);
					float phi = float(y) / float(verItr);
					float xPos = float(std::sin(phi * PIO_PI) * std::sin(theta * 2.f * PIO_PI));
					float yPos = float(std::cos(phi * PIO_PI));
					float zPos = float(std::sin(phi * PIO_PI) * std::cos(theta * 2.f * PIO_PI));

					glm::vec3 normal = glm::normalize(glm::vec3(xPos, yPos, zPos));
					v.Pos = radius * normal;
					v.TexCoord = glm::vec2(theta, 1.f - phi);
					v.Normal = normal;

					subMesh.BoundingBox.Update(v.Pos);
					vertice.push_back(std::move(v));
				}

				// add indices for sphere
				bool pickFirst = (x >= (horItr - 1));
				if (y == 1)
				{
					// add triangles for rendering
					uint32_t ind = GetVertexInd(x, y);
					indice.emplace_back(0);
					indice.emplace_back(ind);
					indice.emplace_back(pickFirst ? GetVertexInd(0, y) : (ind + 1));
				}
				else if (y == verItr)
				{
					uint32_t endInd = total - 1;
					indice.emplace_back(GetVertexInd(x, y - 1));
					indice.emplace_back(endInd);
					indice.emplace_back(GetVertexInd(pickFirst ? 0 : x + 1, y - 1));
				}
				else if (y != 0)
				{
					if (pickFirst)
					{
						uint32_t lastInd = GetVertexInd(x, y - 1);
						uint32_t lastFirstInd = GetVertexInd(0, y - 1);
						uint32_t curInd = GetVertexInd(x, y);
						uint32_t curFirstInd = GetVertexInd(0, y);

						indice.emplace_back(lastInd);
						indice.emplace_back(curInd);
						indice.emplace_back(lastFirstInd);
						indice.emplace_back(lastFirstInd);
						indice.emplace_back(curInd);
						indice.emplace_back(curFirstInd);
					}
					else
					{
						uint32_t lastInd = GetVertexInd(x, y - 1);
						uint32_t curInd = GetVertexInd(x, y);

						indice.emplace_back(lastInd);
						indice.emplace_back(curInd);
						indice.emplace_back(lastInd + 1);
						indice.emplace_back(lastInd + 1);
						indice.emplace_back(curInd);
						indice.emplace_back(curInd + 1);
					}
				}
			}
		}

		for (size_t i = 0; i < indice.size(); i += 3)
		{
			auto& v0 = vertice[indice[i]];
			auto& v1 = vertice[indice[i + 1]];
			auto& v2 = vertice[indice[i + 2]];
			CalcTangent(v0, v1, v2);
		}

		subMesh.VerticeNum = vertice.size();
		subMesh.IndexNum = indice.size();

		return subMesh;
	}

	SubMesh MeshFactory::MakeScreenQuad()
	{
		SubMesh subMesh;
		Vertex v0, v1, v2, v3;

		v0.Pos = glm::vec3(-1.f, 1.f, 0.f);
		v1.Pos = glm::vec3(-1.f, -1.f, 0.f);
		v2.Pos = glm::vec3(1.f, -1.f, 0.f);
		v3.Pos = glm::vec3(1.f, 1.f, 0.f);

		subMesh.BoundingBox.Update(v0.Pos);
		subMesh.BoundingBox.Update(v1.Pos);
		subMesh.BoundingBox.Update(v2.Pos);
		subMesh.BoundingBox.Update(v3.Pos);

		v0.TexCoord = glm::vec2(0.f, 1.f);
		v1.TexCoord = glm::vec2(0.f, 0.f);
		v2.TexCoord = glm::vec2(1.f, 0.f);
		v3.TexCoord = glm::vec2(1.f, 1.f);

		v0.Normal = v1.Normal = v2.Normal = v3.Normal = World::Forward;

		MakeVertexMesh(v0, v1, v2, v3, subMesh);

		auto& vertice = subMesh.Vertice;
		auto& indice = subMesh.Indice;
		for(size_t i = 0; i < indice.size(); i += 3)
		{
			auto& v0 = vertice[indice[i]];
			auto& v1 = vertice[indice[i + 1]];
			auto& v2 = vertice[indice[i + 2]];
			CalcTangent(v0, v1, v2);
		}

		return subMesh;
	}

	SubMesh MeshFactory::MakeDirLightGizmo(float r, float len)
	{
		SubMesh subMesh;

		const size_t itr = 8;
		const float span = 360.f / float(itr);
		float angle{ 0.f };

		auto& vertice = subMesh.Vertice;
		auto& indice = subMesh.Indice;

		vertice.reserve(17);
		vertice.emplace_back(glm::vec3(0.f), glm::vec2(0.5f));

		for(size_t i = 0; i < itr; i++)
		{
			angle = glm::radians(i * span);
			vertice.emplace_back(glm::vec3(r * std::cos(angle), r * std::sin(angle), 0.f), glm::vec2(0.5f));
			vertice.emplace_back(glm::vec3(r * std::cos(angle), r * std::sin(angle), len), glm::vec2(0.5f));
		}

		for(size_t i = 0; i < itr; i++)
		{
			// radial edge
			indice.push_back(0);
			indice.push_back(2 * i + 1);

			// subtense
			if(i == itr - 1)
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

		subMesh.VerticeNum = vertice.size();
		subMesh.IndexNum = indice.size();

		return subMesh;
	}

	SubMesh MeshFactory::MakePointLightGizmo(float r, int32_t itr)
	{
		SubMesh subMesh;
		const float span = 360.f / float(itr);
		float angle{ 0.f };		

		auto& vertice = subMesh.Vertice;
		auto& indice = subMesh.Indice;

		// vertex on plane xoz
		for(size_t i = 0; i < itr; i++)
		{
			angle = glm::radians(i * span);
			vertice.emplace_back(glm::vec3(r * std::sin(angle), 0.f, r * std::cos(angle)), glm::vec2(0.5f));
			indice.emplace_back(i);
			indice.emplace_back((i == (itr - 1)) ? (i + 1 - itr) : (i + 1));
		}

		uint16_t offset = vertice.size();
		// vertex on plane xoy
		for(size_t i = 0; i < itr; i++)
		{
			angle = glm::radians(i * span);
			vertice.emplace_back(glm::vec3(r * std::sin(angle), r * std::cos(angle), 0.f), glm::vec2(0.5f));
			indice.emplace_back(i + offset);
			indice.emplace_back((i == (itr - 1)) ? (i + offset + 1 - itr) : (i + offset + 1));
		}

		subMesh.VerticeNum = vertice.size();
		subMesh.IndexNum = indice.size();

		return subMesh;
	}

	void MeshFactory::MakeVertexMesh(const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3, SubMesh& subMesh)
	{
		auto& vertice = subMesh.Vertice;
		auto& indice = subMesh.Indice;
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

	void MeshFactory::CalcTangent(Vertex& v0, Vertex& v1, Vertex& v2)
	{
		auto edge0 = v1.Pos - v0.Pos;
		auto edge1 = v2.Pos - v0.Pos;
		auto uvDelta0 = v1.TexCoord - v0.TexCoord;
		auto uvDelta1 = v2.TexCoord - v0.TexCoord;

		float f = 1.0f / (uvDelta0.x * uvDelta1.y - uvDelta1.x * uvDelta0.y);
		glm::vec3 tangent, bitan;
		tangent.x = f * (uvDelta1.y * edge0.x - uvDelta0.y * edge1.x);
		tangent.y = f * (uvDelta1.y * edge0.y - uvDelta0.y * edge1.y);
		tangent.z = f * (uvDelta1.y * edge0.z - uvDelta0.y * edge1.z);
		tangent = glm::normalize(tangent);

		bitan.x = f * (-uvDelta1.x * edge0.x + uvDelta0.x * edge1.x);
		bitan.y = f * (-uvDelta1.x * edge0.y + uvDelta0.x * edge1.y);
		bitan.z = f * (-uvDelta1.x * edge0.z + uvDelta0.x * edge1.z);
		bitan = glm::normalize(bitan);

		v0.Tangent = v1.Tangent = v2.Tangent = tangent;
		v0.Bitangent = v1.Bitangent = v2.Bitangent = bitan;
	}
}