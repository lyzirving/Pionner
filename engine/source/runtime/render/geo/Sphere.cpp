#include <cmath>

#include "Sphere.h"

#include "world/comp/MeshComp.h"
#include "world/comp/TransformComp.h"

#include "render/RenderDef.h"
#include "render/resource/ResourceDef.h"

#include "render/rhi/Rhi.h"
#include "render/rhi/DrawCmd.h"

#include "render/shader/ShaderMgr.h"

#include "render/scene/SceneMgr.h"

#include "core/log/LogSystem.h"
#include "core/math/MathLib.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Sphere"

namespace pio
{
	Sphere::Sphere(uint32_t horSegments, uint32_t verSegments, const char *name)
		: Geometry(name)
		, m_horSegmentCnt(horSegments), m_verSegmentCnt(verSegments)
		, m_radius(1.f)
		, m_vertexArray(), m_indiceArray()
	{
		m_type = GEO_TYPE_SPHERE;
		m_mesh = std::shared_ptr<MeshComp>(new MeshComp);
		m_mesh->m_color = glm::vec4(0.392f, 0.392f, 0.392f, 0.6f);
		m_transform = std::shared_ptr<TransformComp>(new TransformComp);
	}

	Sphere::~Sphere()
	{
		std::vector<Vertex>().swap(m_vertexArray);
		std::vector<uint32_t>().swap(m_indiceArray);
	}

	void Sphere::draw(RenderParam &param)
	{
		if (!m_mesh)
			return;

		initialize(param);

		std::shared_ptr<Shader> shader{ nullptr };
		if (!dealShader(param, shader))
			return;

		auto rhi = param.rhi;
		gfx::DepthTest depth = gfx::DepthTest::common();
		gfx::Blend blend = gfx::Blend::common();
		gfx::CullFace cull = gfx::CullFace::common();
		rhi->setCullMode(cull);
		rhi->setDepthMode(depth);
		rhi->setBlendMode(blend);

		auto cmd = param.rhi->getDrawCmd();
		cmd->drawGeometry(*this, param);

		shader->use(false);
	}

	void Sphere::initialize(RenderParam &param)
	{
		if (!m_mesh->m_initialized)
		{
			m_vertexArray.clear();
			m_indiceArray.clear();
			buildData(m_vertexArray, m_indiceArray);
			m_mesh->initialize(m_vertexArray, m_indiceArray);
		}
	}

	bool Sphere::dealShader(RenderParam &param, std::shared_ptr<Shader> &shader)
	{
		if (!m_mesh || !m_transform)
		{
			LOG_ERR("components are invalid");
			return false;
		}

		shader = param.shaderMgr->get(SHADER_TYPE_COLOR_GEOMETRY);

		if (!shader)
		{
			LOG_ERR("fail to get color geometry shader");
			return false;
		}

		shader->use(true);

		shader->setVec4("u_color", m_mesh->m_color);
		shader->setMat4("u_modelMat", m_transform->getMat());
		shader->setMat4("u_viewMat", param.sceneMgr->m_camera->getViewMat());
		shader->setMat4("u_prjMat", param.sceneMgr->m_frustum->getPerspectMat());
		shader->setInt("u_calcShadow", 0);

		return true;
	}

	void Sphere::setRadius(float radius)
	{
		if (m_mesh && m_mesh->m_initialized)
		{
			LOG_ERR("mesh has been initialized, fail to set radius");
			return;
		}
		m_radius = radius;
	}

	void Sphere::buildData(std::vector<Vertex> &vertexArray, std::vector<uint32_t> &indiceArray)
	{
		for (int y = 0; y <= m_verSegmentCnt; y++)
		{
			for (int x = 0; x < m_horSegmentCnt; x++)
			{
				// build vertex for sphere
				if ((y == 0 && x == 0) || (y == m_verSegmentCnt && x == 0) || (y != 0 && y != m_verSegmentCnt))
				{
					Vertex vertex{};
					// theta is the angle between vector projected on x-z plane and +z
					// phi is the angle between vector and +y
					float theta = float(x) / float(m_horSegmentCnt);
					float phi = float(y) / float(m_verSegmentCnt);
					auto xPos = float(std::sin(phi * MATH_PI) * std::sin(theta * 2 * MATH_PI));
					auto yPos = float(std::cos(phi * MATH_PI));
					auto zPos = float(std::sin(phi * MATH_PI) * std::cos(theta * 2 * MATH_PI));

					glm::vec3 vec = glm::normalize(glm::vec3(xPos, yPos, zPos));
					vertex.pos = m_radius * vec;
					vertex.normal = vec;
					vertexArray.push_back(std::move(vertex));
				}

				// add indices for sphere
				bool pickFirst = (x >= (m_horSegmentCnt - 1));
				if (y == 1)
				{
					// add triangles for rendering
					uint32_t ind = curVertexInd(x, y);
					indiceArray.push_back(0);
					indiceArray.push_back(ind);
					indiceArray.push_back(pickFirst ? curVertexInd(0, y) : (ind + 1));
				}
				else if (y == m_verSegmentCnt)
				{
					// add triangles for rendering
					uint32_t endInd = totalVertexCnt() - 1;
					indiceArray.push_back(curVertexInd(x, y - 1));
					indiceArray.push_back(endInd);
					indiceArray.push_back(curVertexInd(pickFirst ? 0 : x + 1, y - 1));
				}
				else if (y != 0)
				{
					// add rectangles for rendering
					if (pickFirst)
					{
						uint32_t lastInd = curVertexInd(x, y - 1);
						uint32_t lastFirstInd = curVertexInd(0, y - 1);
						uint32_t curInd = curVertexInd(x, y);
						uint32_t curFirstInd = curVertexInd(0, y);

						indiceArray.push_back(lastInd);
						indiceArray.push_back(curInd);
						indiceArray.push_back(lastFirstInd);

						indiceArray.push_back(lastFirstInd);
						indiceArray.push_back(curInd);
						indiceArray.push_back(curFirstInd);
					}
					else
					{
						uint32_t lastInd = curVertexInd(x, y - 1);
						uint32_t curInd = curVertexInd(x, y);

						indiceArray.push_back(lastInd);
						indiceArray.push_back(curInd);
						indiceArray.push_back(lastInd + 1);

						indiceArray.push_back(lastInd + 1);
						indiceArray.push_back(curInd);
						indiceArray.push_back(curInd + 1);
					}
				}
			}
		}
		LOG_DEBUG("indices count[%lu], supposed indices count[%u], vertices cnt[%lu], supposed vertices cnt[%u]",
				  indiceArray.size(), totalIndicesCnt(),
				  vertexArray.size(), totalVertexCnt());
	}

	uint32_t Sphere::curVertexInd(uint32_t x, uint32_t y)
	{
		if (x == 0 && y == 0)
			return 0;
		else if (x == 0 && y == m_verSegmentCnt)
			return totalVertexCnt() - 1;
		else if (y != 0 && y != m_verSegmentCnt)
			return (1 + (y - 1) * m_horSegmentCnt + x);

		LOG_ERR("out of bounds err for ind[%u, %u], segments[%u, %u], return 0", x, y, m_horSegmentCnt, m_verSegmentCnt);
		return 0;
	}

	uint32_t Sphere::totalVertexCnt()
	{
		return 2 + (m_verSegmentCnt - 1) * m_horSegmentCnt;
	}

	uint32_t Sphere::totalIndicesCnt()
	{
		return (2 * m_horSegmentCnt * 3 + (m_verSegmentCnt - 2) * m_horSegmentCnt * 6);
	}

	template<>
	bool Geometry::is<Sphere>()
	{
		return m_type == GEO_TYPE_SPHERE;
	}

	template<>
	Sphere *Geometry::as<Sphere>()
	{
		if (is<Sphere>())
		{
			return static_cast<Sphere *>(this);
		}
		return nullptr;
	}
}