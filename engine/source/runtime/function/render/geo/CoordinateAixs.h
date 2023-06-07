#ifndef __RENDER_COORDINATE_AXIS_H__
#define __RENDER_COORDINATE_AXIS_H__

#include "GeometryGroup.h"

namespace Pionner
{
	enum AxisType : uint8_t
	{
		AXIS_X,
		AXIS_Y,
		AXIS_Z,
		AXIS_COUNT
	};

	class CoordinateAxis : public GeometryGroup
	{
	public:
		CoordinateAxis(const char *name);
		virtual ~CoordinateAxis();

		virtual void initialize(RenderParam &param) override;

	private:
		bool m_initialized;
	};

	template<>
	bool Geometry::is<CoordinateAxis>();

	template<>
	CoordinateAxis *Geometry::as<CoordinateAxis>();

}

#endif