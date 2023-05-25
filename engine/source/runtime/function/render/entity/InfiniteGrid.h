#ifndef __INFINITE_GRID_H__
#define __INFINITE_GRID_H__

#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace Pionner
{
	class  Rhi;
	class  MeshComp;
	struct RenderParam;
	struct Vertex;

	class InfiniteGrid
	{
	public:
		InfiniteGrid(const std::shared_ptr<Rhi> &rhi);
		~InfiniteGrid();

		void draw(RenderParam &param);
		void initialize(RenderParam &param);

	private:
		static void buildGridData(std::vector<Vertex> &vertexArray,
								  std::vector<uint32_t> &indiceArray);

	private:
		static const float g_dist;

		std::shared_ptr<Rhi>      m_rhi;
		std::shared_ptr<MeshComp> m_meshComp;
		std::vector<Vertex>       m_vertexArray;
		std::vector<uint32_t>     m_indiceArray;
	};
}

#endif