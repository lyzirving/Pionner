#include "Geometry.h"

#include "gfx/rhi/Material.h"
#include "gfx/rhi/VertexBuffer.h"
#include "gfx/rhi/IndexBuffer.h"

namespace pio
{
	void Geometry::setColor(const glm::vec3 &color)
	{
		m_materials[0]->set(MaterialAttrs::MU_AlbedoColor, color);
	}

	Ref<Material> Geometry::getMaterial() const
	{
		return m_materials[0];
	}

	void Geometry::flush()
	{
		if (!needUpdate())
			return;

		if (m_submeshes.empty() || m_vertices.empty() || m_indices.empty())
		{
			LOGE("[%s] invalid state", m_name.c_str());
			return;
		}

		for (uint32_t i = 0; i < m_submeshes.size(); i++)
		{
			Submesh &submesh = m_submeshes[i];
			if (submesh.VertexBuffer.use_count() == 0 || submesh.IndexBuffer.use_count() == 0)
			{
				LOGE("[%s] invalid buffer", m_name.c_str());
				continue;
			}
			submesh.VertexBuffer->setData(&(m_vertices[submesh.VertexOffset]), submesh.VertexCount * sizeof(Vertex));
			submesh.IndexBuffer->setData(&(m_indices[submesh.IndexOffset]), submesh.IndexCount * sizeof(Index), submesh.IndexCount * 3);
		}

		m_invalidate = false;
	}

	void Sphere::setRadius(float radius)
	{
		if (!Math::Equal(m_radius, radius))
		{
			m_radius = radius;
			build();
		}
	}

	void Sphere::build()
	{
		m_triangleCache.clear();
		m_vertices.clear();
		m_indices.clear();

		// Only one mesh for a Sphere
		// Should not clear submesh, for there might be synchronous issue
		if (m_submeshes.empty()) 
		{ 
			m_submeshes.emplace_back(); 
			m_submeshes[0].MeshName = m_name + "@mesh";
		}

		Submesh &submesh = m_submeshes.back();
		submesh.VertexOffset = 0;
		submesh.IndexOffset = 0;
		submesh.MaterialIndex = 0;// one mesh has one material
		submesh.BoundingBox = AABB(glm::vec3(-m_radius), glm::vec3(m_radius));
		m_boundingBox = submesh.BoundingBox;

		uint32_t totalCnt = 2 + (m_verItrCnt - 1) * m_horItrCnt;

		auto CurVertexInd = [&](uint32_t pX, uint32_t pY)
		{
			if (pX == 0 && pY == 0)
				return 0U;
			else if (pX == 0 && pY == m_verItrCnt)
				return totalCnt - 1;
			else if (pY != 0 && pY != m_verItrCnt)
				return (1 + (pY - 1) * m_horItrCnt + pX);

			return 0U;
		};

		for (uint32_t y = 0; y <= m_verItrCnt; y++)
		{
			for (uint32_t x = 0; x < m_horItrCnt; x++)
			{
				// build vertex for sphere
				if ((y == 0 && x == 0) || (y == m_verItrCnt && x == 0) || (y != 0 && y != m_verItrCnt))
				{
					Vertex vertex{};
					// theta is the angle between vector projected on x-z plane and +z
					// phi is the angle between vector and +y
					float theta = float(x) / float(m_horItrCnt);
					float phi = float(y) / float(m_verItrCnt);
					float xPos = float(std::sin(phi * PIO_PI) * std::sin(theta * 2 * PIO_PI));
					float yPos = float(std::cos(phi * PIO_PI));
					float zPos = float(std::sin(phi * PIO_PI) * std::cos(theta * 2 * PIO_PI));

					glm::vec3 n = glm::normalize(glm::vec3(xPos, yPos, zPos));
					vertex.Position = m_radius * n;
					vertex.Normal = n;
					vertex.Texcoord = glm::vec2(theta, phi);

					m_vertices.push_back(vertex);
				}

				// add indices for sphere
				bool pickFirst = (x >= (m_horItrCnt - 1));
				if (y == 1)
				{
					// add triangles for rendering
					uint32_t ind = CurVertexInd(x, y);
					Index faceInd(0, ind, pickFirst ? CurVertexInd(0, y) : (ind + 1));
					m_indices.push_back(faceInd);
				}
				else if (y == m_verItrCnt)
				{
					uint32_t endInd = totalCnt - 1;
					Index faceInd(CurVertexInd(x, y - 1), endInd, CurVertexInd(pickFirst ? 0 : x + 1, y - 1));
					m_indices.push_back(faceInd);
				}
				else if (y != 0)
				{
					if (pickFirst)
					{
						uint32_t lastInd = CurVertexInd(x, y - 1);
						uint32_t lastFirstInd = CurVertexInd(0, y - 1);
						uint32_t curInd = CurVertexInd(x, y);
						uint32_t curFirstInd = CurVertexInd(0, y);

						m_indices.emplace_back(lastInd, curInd, lastFirstInd);
						m_indices.emplace_back(lastFirstInd, curInd, curFirstInd);
					}
					else
					{
						uint32_t lastInd = CurVertexInd(x, y - 1);
						uint32_t curInd = CurVertexInd(x, y);

						m_indices.emplace_back(lastInd, curInd, lastInd + 1);
						m_indices.emplace_back(lastInd + 1, curInd, curInd + 1);
					}
				}
			}
		}

		submesh.VertexCount = m_vertices.size();
		uint32_t faceNum = submesh.IndexCount = m_indices.size();

		for (uint32_t i = 0; i < faceNum; i++)
		{
			m_triangleCache[i].emplace_back(m_vertices[m_indices[i].V0 + submesh.VertexOffset],
											m_vertices[m_indices[i].V1 + submesh.VertexOffset],
											m_vertices[m_indices[i].V2 + submesh.VertexOffset]);
		}

		invalidate();
	}

	void Circle::setRadius(float radius)
	{
		if (!Math::Equal(m_radius, radius))
		{
			m_radius = radius;
			build();
		}
	}

	void Circle::build()
	{
		m_triangleCache.clear();
		m_vertices.clear();
		m_indices.clear();

		// Only one mesh for a Circle
		// Should not clear submesh, for there might be synchronous issue
		if (m_submeshes.empty())
		{
			m_submeshes.emplace_back();
			m_submeshes[0].MeshName = m_name + "@mesh";
		}

		Submesh &submesh = m_submeshes.back();
		submesh.VertexOffset = 0;
		submesh.IndexOffset = 0;
		submesh.MaterialIndex = 0;// one mesh has one material
		submesh.BoundingBox = AABB(glm::vec3(-m_radius, -m_radius, 0.f), glm::vec3(m_radius, m_radius, 0.f));
		m_boundingBox = submesh.BoundingBox;

		const float span = 360.f / float(m_itrCnt);		
		uint32_t index{ 0 };

		auto __makeGeo = [&](float _angle0, float _angle1, uint32_t j) 
		{
			glm::vec3 anchor0 = { m_radius * std::cosf(glm::radians(_angle0)), m_radius * std::sinf(glm::radians(_angle0)), 0.f };
			glm::vec3 anchor1 = { m_radius * std::cosf(glm::radians(_angle1)), m_radius * std::sinf(glm::radians(_angle1)), 0.f };

			glm::vec3 dir0 = glm::normalize(anchor0 - glm::vec3(0.f));
			glm::vec3 dir1 = glm::normalize(anchor1 - glm::vec3(0.f));

			Vertex v0;
			v0.Position = anchor0 - dir0 * m_ringWidth;
			v0.Normal = dir0;
			v0.Texcoord = glm::vec2(float(j) / float(m_itrCnt), 0.f);

			Vertex v1;
			v1.Position = anchor0 + dir0 * m_ringWidth;
			v1.Normal = dir0;
			v1.Texcoord = glm::vec2(float(j) / float(m_itrCnt), 1.f);

			Vertex v2;
			v2.Position = anchor1 - dir1 * m_ringWidth;
			v2.Normal = dir1;
			v2.Texcoord = glm::vec2(float(j + 1) / float(m_itrCnt), 0.f);

			Vertex v3;
			v3.Position = anchor1 + dir1 * m_ringWidth;
			v3.Normal = dir1;
			v3.Texcoord = glm::vec2(float(j + 1) / float(m_itrCnt), 1.f);

			m_vertices.push_back(std::move(v0));
			m_vertices.push_back(std::move(v1));
			m_vertices.push_back(std::move(v2));
			m_vertices.push_back(std::move(v3));

			m_indices.emplace_back(index, index + 1, index + 2);
			m_indices.emplace_back(index + 2, index + 1, index + 3);
		};


		// Vertex are organized on XY plane in counter-clockwise order
		for (uint32_t i = 0; i < (m_itrCnt - 1); i++)
		{
			float angle0 = i * span;
			float angle1 = (i + 1) * span;
			__makeGeo(angle0, angle1, i);
			index += 4;
		}

		float angle0 = (m_itrCnt - 1) * span;
		float angle1 = 360.f;
		__makeGeo(angle0, angle1, m_itrCnt - 1);

		submesh.VertexCount = m_vertices.size();
		uint32_t faceNum = submesh.IndexCount = m_indices.size();

		for (uint32_t i = 0; i < faceNum; i++)
		{
			m_triangleCache[i].emplace_back(m_vertices[m_indices[i].V0 + submesh.VertexOffset],
											m_vertices[m_indices[i].V1 + submesh.VertexOffset],
											m_vertices[m_indices[i].V2 + submesh.VertexOffset]);
		}

		invalidate();
	}

	template<>
	bool Asset::is<Geometry>() const { return getAssetType() > AssetType::GeoBegin && getAssetType() < AssetType::GeoEnd; }

	template<>
	bool Asset::is<Cylinder>() const { return getAssetType() == AssetType::Cylinder; }

	template<>
	bool Asset::is<Cone>() const { return getAssetType() == AssetType::Cone; }

	template<>
	bool Asset::is<Arrow3D>() const { return getAssetType() == AssetType::Arrow; }

	template<>
	bool Asset::is<Sphere>() const { return getAssetType() == AssetType::Sphere; }

	template<>
	bool Asset::is<Circle>() const { return getAssetType() == AssetType::Circle; }	
}