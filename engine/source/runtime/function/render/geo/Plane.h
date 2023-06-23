#ifndef __RENDER_GEO_PLANE_H__
#define __RENDER_GEO_PLANE_H__

#include "Geometry.h"

namespace Pionner
{
	class Plane : public Geometry
	{
	public:
		Plane();
		Plane(const std::string &name);
		virtual ~Plane();

		virtual void draw(RenderParam &param) override;
		virtual void initialize(RenderParam &param) override;
		virtual bool dealShader(RenderParam &param, std::shared_ptr<Shader> &shader) override;
	};
}

#endif