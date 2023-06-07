#include "CoordinateAixs.h"
#include "Cylinder.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "CoordinateAxis"

namespace Pionner
{
	CoordinateAxis::CoordinateAxis(const char *name) : GeometryGroup(name), m_initialized(false)
	{
	}

	CoordinateAxis::~CoordinateAxis() = default;

	void CoordinateAxis::initialize(RenderParam &param)
	{
		if (!m_initialized)
		{
			auto xAxis = std::shared_ptr<Geometry>(new Cylinder("x axis"));
			auto yAxis = std::shared_ptr<Geometry>(new Cylinder("y axis"));
			auto zAxis = std::shared_ptr<Geometry>(new Cylinder("z axis"));

			xAxis->initialize(param);
			yAxis->initialize(param);
			zAxis->initialize(param);

			if (xAxis->isInitialized() && yAxis->isInitialized() && zAxis->isInitialized())
			{
				m_geometrys.clear();
				/*xAxis->rotate(-90.f, 0.f, 0.f, 1.f);
				xAxis->translate(xAxis->getHeight() * 0.5f, 0.f, 0.f);
				xAxis->setColor(glm::vec4(1.f, 0.f, 0.f, 1.f));

				yAxis->translate(0.f, yAxis->getHeight() * 0.5f, 0.f);
				yAxis->setColor(glm::vec4(0.f, 1.f, 0.f, 1.f));

				zAxis->rotate(90.f, 1.f, 0.f, 0.f);
				zAxis->translate(0.f, 0.f, zAxis->getHeight() * 0.5f);
				zAxis->setColor(glm::vec4(0.f, 0.f, 1.f, 1.f));*/

				m_geometrys.push_back(xAxis);
				m_geometrys.push_back(yAxis);
				m_geometrys.push_back(zAxis);
				m_initialized = true;
				LOG_DEBUG("[%s] succed to init", m_name.c_str());
			}
		}
	}

}