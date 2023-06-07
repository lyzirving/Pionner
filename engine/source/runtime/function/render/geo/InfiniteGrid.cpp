#include "InfiniteGrid.h"

#include "function/render/rhi/Rhi.h"
#include "function/render/rhi/DrawCmd.h"

#include "function/render/resource/RenderResourceMgr.h"
#include "function/render/resource/buffer/GfxBuffer.h"

#include "function/render/scene/SceneMgr.h"
#include "function/render/scene/Camera.h"
#include "function/render/scene/Frustum.h"

#include "function/render/RenderDef.h"
#include "function/render/RenderSystem.h"

#include "function/global/GlobalContext.h"

#include "function/framework/comp/MeshComp.h"

namespace Pionner
{
	const float InfiniteGrid::g_dist = 1000.f;

	InfiniteGrid::InfiniteGrid() : Geometry("infinite grid"), m_vertexArray(), m_indiceArray()
	{
		m_mesh = std::shared_ptr<MeshComp>(new MeshComp);
		m_type = GEO_TYPE_INFINITE_GRID;
	}

	InfiniteGrid::~InfiniteGrid()
	{
		std::vector<Vertex>().swap(m_vertexArray);
		std::vector<uint32_t>().swap(m_indiceArray);
	}

	void InfiniteGrid::draw(RenderParam &param)
	{
		if (m_mesh && m_mesh->m_initialized)
		{
			std::shared_ptr<DrawCmd> drawCmd = param.rhi->getDrawCmd();
			drawCmd->drawInfiniteGrid(m_mesh, param);
		}
	}

	void InfiniteGrid::initialize(RenderParam &param)
	{
		if (!m_mesh->m_initialized)
		{
			m_vertexArray.clear();
			m_indiceArray.clear();
			buildGridData(m_vertexArray, m_indiceArray);
			m_mesh->initialize(m_vertexArray, m_indiceArray);
			m_mesh->m_color = glm::vec4(0.29f, 0.29f, 0.29f, 1.f);
		}
	}

	void InfiniteGrid::buildGridData(std::vector<Vertex> &vertexArray, std::vector<uint32_t> &indiceArray)
	{
		Vertex v;
		v.pos = { -g_dist, 0.f, -g_dist };
		vertexArray.push_back(v);

		v.pos = { -g_dist, 0.f, g_dist };
		vertexArray.push_back(v);

		v.pos = { g_dist, 0.f, -g_dist };
		vertexArray.push_back(v);

		v.pos = { g_dist, 0.f, g_dist };
		vertexArray.push_back(v);

		indiceArray.push_back(0);
		indiceArray.push_back(1);
		indiceArray.push_back(2);
		indiceArray.push_back(2);
		indiceArray.push_back(1);
		indiceArray.push_back(3);
	}

	template<>
	bool Geometry::is<InfiniteGrid>()
	{
		return m_type == GEO_TYPE_INFINITE_GRID;
	}

	template<>
	InfiniteGrid *Geometry::as<InfiniteGrid>()
	{
		if (is<InfiniteGrid>())
		{
			return static_cast<InfiniteGrid *>(this);
		}
		return nullptr;
	}
}