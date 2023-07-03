#include "CoordinateAixs.h"
#include "Cylinder.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "CoordinateAxis"

namespace pio
{
	CoordinateAxis::CoordinateAxis(const char *name) : GeometryGroup(name), m_initialized(false)
	{
		m_type = GEO_TYPE_COORD_AXIS;
	}

	CoordinateAxis::~CoordinateAxis() = default;

	void CoordinateAxis::initialize(RenderParam &param)
	{
		if (!m_initialized)
		{
			auto xAxis = std::shared_ptr<Geometry>(new Cylinder("x axis"));
			auto yAxis = std::shared_ptr<Geometry>(new Cylinder("y axis"));
			auto zAxis = std::shared_ptr<Geometry>(new Cylinder("z axis"));

			auto *pX = xAxis->as<Cylinder>();
			auto *pY = yAxis->as<Cylinder>();
			auto *pZ = zAxis->as<Cylinder>();

			pX->setRadius(0.03f);
			pY->setRadius(0.03f);
			pZ->setRadius(0.03f);

			pX->rotate(-90.f, 0.f, 0.f, 1.f);
			pX->translate(pX->getHeight() * 0.5f, 0.f, 0.f);
			pX->setColor(glm::vec4(1.f, 0.f, 0.f, 1.f));

			pY->translate(0.f, pY->getHeight() * 0.5f, 0.f);
			pY->setColor(glm::vec4(0.f, 1.f, 0.f, 1.f));

			pZ->rotate(90.f, 1.f, 0.f, 0.f);
			pZ->translate(0.f, 0.f, pZ->getHeight() * 0.5f);
			pZ->setColor(glm::vec4(0.f, 0.f, 1.f, 1.f));

			xAxis->initialize(param);
			yAxis->initialize(param);
			zAxis->initialize(param);

			if (xAxis->isInitialized() && yAxis->isInitialized() && zAxis->isInitialized())
			{
				m_geometrys.clear();
				m_geometrys.push_back(xAxis);
				m_geometrys.push_back(yAxis);
				m_geometrys.push_back(zAxis);
				m_initialized = true;
				LOG_DEBUG("[%s] succed to init", m_name.c_str());
			}
		}
	}

	template<>
	bool Geometry::is<CoordinateAxis>()
	{
		return m_type == GEO_TYPE_COORD_AXIS;
	}

	template<>
	CoordinateAxis *Geometry::as<CoordinateAxis>()
	{
		if (is<CoordinateAxis>())
		{
			return static_cast<CoordinateAxis *>(this);
		}
		return nullptr;
	}

}