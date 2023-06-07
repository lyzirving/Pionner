#ifndef __GEO_INFINITE_GRID_H__
#define __GEO_INFINITE_GRID_H__

#include <glm/glm.hpp>

#include <vector>

#include "function/render/geo/Geometry.h"

namespace Pionner
{
	struct Vertex;

	class InfiniteGrid : public Geometry
	{
	public:
		InfiniteGrid();
		virtual ~InfiniteGrid();

		virtual void draw(RenderParam &param) override;
		virtual void initialize(RenderParam &param) override;

	private:
		static void buildGridData(std::vector<Vertex> &vertexArray,
								  std::vector<uint32_t> &indiceArray);

	private:
		static const float g_dist;

		std::vector<Vertex>   m_vertexArray;
		std::vector<uint32_t> m_indiceArray;
	};
}

#endif