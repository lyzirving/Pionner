#include "function/render/geo/GeometryGroup.h"

namespace Pionner
{
	Pionner::GeometryGroup::GeometryGroup(const char *name) : Geometry(name), m_geometrys()
	{
		m_type = GEO_TYPE_GROUP;
	}

	Pionner::GeometryGroup::~GeometryGroup()
	{
		auto itr = m_geometrys.begin();
		while (itr != m_geometrys.end())
		{
			(*itr).reset();
			itr = m_geometrys.erase(itr);
		}
	}

	void GeometryGroup::draw(RenderParam &param)
	{
		for (size_t i = 0; i < m_geometrys.size(); i++)
		{
			if (m_geometrys[i])
				m_geometrys[i]->draw(param);
		}
	}

	void GeometryGroup::initialize(RenderParam &param)
	{
	}

	template<>
	bool Geometry::is<GeometryGroup>()
	{
		return m_type == GEO_TYPE_GROUP;
	}

	template<>
	GeometryGroup *Geometry::as<GeometryGroup>()
	{
		if (is<GeometryGroup>())
		{
			return static_cast<GeometryGroup *>(this);
		}
		return nullptr;
	}
}
