#ifndef __GEO_CYLINDER_H__
#define __GEO_CYLINDER_H__

#include "function/render/geo/GeoObj.h"

namespace Pionner
{
	class Cylinder : public GeoObj
	{
	public:
		Cylinder();
		~Cylinder();

		virtual void draw(RenderParam &param) override;
		virtual void initialize(RenderParam &param) override;

	private:

	};
}

#endif