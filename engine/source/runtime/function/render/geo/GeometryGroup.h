#ifndef __RENDER_GEOMETRY_GROUP_H__
#define __RENDER_GEOMETRY_GROUP_H__

#include <vector>

#include "function/render/geo/Geometry.h"

namespace Pionner
{
	class GeometryGroup : public Geometry
	{
	public:
		GeometryGroup(const char *name);
		virtual ~GeometryGroup();

		virtual void draw(RenderParam &param) override;
		virtual void initialize(RenderParam &param) override;
		virtual bool dealShader(RenderParam &param, std::shared_ptr<Shader> &shader) override;

	protected:
		std::vector<std::shared_ptr<Geometry>> m_geometrys;
	};

	template<>
	bool Geometry::is<GeometryGroup>();

	template<>
	GeometryGroup *Geometry::as<GeometryGroup>();
}

#endif