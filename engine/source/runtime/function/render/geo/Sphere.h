#ifndef __RENDER_GEO_SPHERE_H__
#define __RENDER_GEO_SPHERE_H__

#include <vector>

#include "Geometry.h"

namespace Pionner
{
	struct Vertex;

	class Sphere : public Geometry
	{
	public:
		Sphere(uint32_t horSegments = 50, uint32_t verSegments = 50, const char *name = nullptr);
		virtual ~Sphere();

		virtual void draw(RenderParam &param) override;
		virtual void initialize(RenderParam &param) override;

		inline float getRadius() const { return m_radius; }

		void setRadius(float radius);

	protected:
		virtual bool dealShader(RenderParam &param, std::shared_ptr<Shader> &shader) override;

	private:
		void buildData(std::vector<Vertex> &vertexArray, std::vector<uint32_t> &indiceArray);
		uint32_t curVertexInd(uint32_t x, uint32_t y);
		uint32_t totalVertexCnt();
		uint32_t totalIndicesCnt();

	private:
		uint32_t m_horSegmentCnt, m_verSegmentCnt;
		float    m_radius;
		std::vector<Vertex>   m_vertexArray;
		std::vector<uint32_t> m_indiceArray;
	};

	template<>
	bool Geometry::is<Sphere>();

	template<>
	Sphere *Geometry::as<Sphere>();
}

#endif