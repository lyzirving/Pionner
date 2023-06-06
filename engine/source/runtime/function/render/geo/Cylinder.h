#ifndef __GEO_CYLINDER_H__
#define __GEO_CYLINDER_H__

#include <vector>
#include <glm/glm.hpp>

#include "function/render/geo/Geometry.h"

namespace Pionner
{
	struct Vertex;

	class Cylinder : public Geometry
	{
	public:
		Cylinder();
		~Cylinder();

		virtual void draw(RenderParam &param) override;
		virtual void initialize(RenderParam &param) override;

		void setColor(const glm::vec4 &color);

	private:
		void buildData(std::vector<Vertex> &vertexArray,
					   std::vector<uint32_t> &indiceArray);

	private:
		glm::vec3 m_center;
		float m_radius, m_height;
		uint32_t m_itrCnt;
		std::vector<Vertex>   m_vertexArray;
		std::vector<uint32_t> m_indiceArray;
	};
}

#endif