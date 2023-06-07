#include <cmath>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Cylinder.h"

#include "function/framework/comp/MeshComp.h"
#include "function/framework/comp/TransformComp.h"

#include "function/render/RenderDef.h"
#include "function/render/resource/ResourceDef.h"

#include "function/render/rhi/Rhi.h"
#include "function/render/rhi/DrawCmd.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Cylinder"

namespace Pionner
{
	Cylinder::Cylinder() : Geometry("cylinder")
		, m_center(0.f, 0.f, 0.f), m_radius(0.05f), m_height(0.5f)
		, m_itrCnt(50)
		, m_vertexArray(), m_indiceArray()
	{
		m_mesh = std::shared_ptr<MeshComp>(new MeshComp);
		m_mesh->m_color = glm::vec4(1.f, 0.f, 0.f, 1.f);

		m_transform = std::shared_ptr<TransformComp>(new TransformComp);
	}

	Cylinder::Cylinder(const char *name) : Geometry(name)
		, m_center(0.f, 0.f, 0.f), m_radius(0.05f), m_height(0.5f)
		, m_itrCnt(50)
		, m_vertexArray(), m_indiceArray()
	{
		m_mesh = std::shared_ptr<MeshComp>(new MeshComp);
		m_mesh->m_color = glm::vec4(1.f, 0.f, 0.f, 1.f);

		m_transform = std::shared_ptr<TransformComp>(new TransformComp);
	}

	Cylinder::~Cylinder()
	{
		std::vector<Vertex>().swap(m_vertexArray);
		std::vector<uint32_t>().swap(m_indiceArray);
	}

	void Cylinder::draw(RenderParam &param)
	{
		if (!m_mesh)
			return;

		initialize(param);

		auto cmd = param.rhi->getDrawCmd();
		cmd->drawColorGeometry(*this, param);
	}

	void Cylinder::initialize(RenderParam &param)
	{
		if (!m_mesh->m_initialized)
		{
			m_vertexArray.clear();
			m_indiceArray.clear();
			buildData(m_vertexArray, m_indiceArray);
			m_mesh->initialize(m_vertexArray, m_indiceArray);
		}
	}

	bool Cylinder::isInitialized()
	{
		return m_mesh && m_transform && m_mesh->m_initialized;
	}

	void Cylinder::setColor(const glm::vec4 &color)
	{
		if (m_mesh)
			m_mesh->m_color = color;
	}

	void Cylinder::translate(float x, float y, float z)
	{
		if (m_transform)
			m_transform->translate(x, y, z);
	}

	void Cylinder::rotate(float angle, float x, float y, float z)
	{
		if (m_transform)
			m_transform->rotate(angle, x, y, z);
	}

	void Cylinder::buildData(std::vector<Vertex> &vertexArray, std::vector<uint32_t> &indiceArray)
	{
		// Step 1. build pts on top and bottom circular surfaces
		float perAngle = 360.f / float(m_itrCnt);
		float curAngle{ 0.f };
		std::vector<Vertex> topSurfacePts{}, bottomSurfacePts;
		glm::vec3 up{ 0.f, 0.5f * m_height, 0.f }, down{ 0.f, -0.5f * m_height, 0.f };
		// Note we only add m_itrCnt - 1 pts.
		for (uint32_t i = 0; i < m_itrCnt - 1; i++)
		{
			Vertex topPt{}, bottomPt{};
			glm::vec3 pos{ 0.f };

			pos.x = std::sin(glm::radians(curAngle)) * m_radius;
			pos.z = std::cos(glm::radians(curAngle)) * m_radius;

			topPt.pos = pos + up + m_center;
			topPt.normal = topPt.pos - m_center;
			topSurfacePts.emplace_back(std::move(topPt));

			bottomPt.pos = pos + down + m_center;
			bottomPt.normal = bottomPt.pos - m_center;
			bottomSurfacePts.emplace_back(std::move(bottomPt));

			curAngle += perAngle;
		}

		// Step 2. use the first pt as the last one, to make it a closure.
		glm::vec3 pos{ 0.f };
		Vertex topCenter{}, bottomCenter{};
		topCenter.pos = m_center + up;
		topCenter.normal = up;

		bottomCenter.pos = m_center + down;
		bottomCenter.normal = down;

		// index: 0 ~ (m_itrCnt - 2)
		vertexArray.insert(vertexArray.end(), topSurfacePts.begin(), topSurfacePts.end());
		// index: (m_itrCnt - 1) ~ (2 * m_itrCnt - 3)
		vertexArray.insert(vertexArray.end(), bottomSurfacePts.begin(), bottomSurfacePts.end());
		// index: 2 * m_itrCnt - 2
		vertexArray.push_back(topCenter);
		// index: 2 * m_itrCnt - 1
		vertexArray.push_back(bottomCenter);

		// Step 3. make index with the points added
		uint32_t topCenterInd = 2 * m_itrCnt - 2;
		uint32_t bottomCenterInd = 2 * m_itrCnt - 1;

		uint32_t limit = m_itrCnt - 2;
		for (uint32_t i = 0; i <= limit; i++)
		{
			// top surface's point-index
			indiceArray.push_back((i == limit) ? 0 : (i + 1));
			indiceArray.push_back(topCenterInd);
			indiceArray.push_back(i);

			// side surface's point-index
			if (i != limit)
			{
				indiceArray.push_back(i + 1);
				indiceArray.push_back(i);
				indiceArray.push_back(i + m_itrCnt - 1);

				indiceArray.push_back(i + 1);
				indiceArray.push_back(i + m_itrCnt - 1);
				indiceArray.push_back(i + m_itrCnt);
			}
			else
			{
				indiceArray.push_back(0);
				indiceArray.push_back(i);
				indiceArray.push_back(i + m_itrCnt - 1);

				indiceArray.push_back(0);
				indiceArray.push_back(i + m_itrCnt - 1);
				indiceArray.push_back(m_itrCnt - 1);
			}
		}

		limit = 2 * m_itrCnt - 3;
		for (uint32_t i = m_itrCnt - 1; i <= limit; i++)
		{
			// bottom surface's point-index
			indiceArray.push_back((i == limit) ? (m_itrCnt - 1) : (i + 1));
			indiceArray.push_back(bottomCenterInd);
			indiceArray.push_back(i);
		}
	}
}