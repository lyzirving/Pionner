#include "function/render/geo/Geometry.h"

#include "function/framework/comp/MeshComp.h"
#include "function/framework/comp/TransformComp.h"

namespace Pionner
{
	Geometry::Geometry(const char *name)
		: m_type(GEO_TYPE_NONE), m_name(name)
		, m_mesh(nullptr), m_transform(nullptr)
	{
	}

	Geometry::~Geometry()
	{
		m_mesh.reset();
		m_transform.reset();
	}

	bool Geometry::isInitialized()
	{
		return false;
	}

	void Geometry::translate(float x, float y, float z)
	{
		if (m_transform)
			m_transform->translate(x, y, z);
	}

	void Geometry::rotate(float angle, float x, float y, float z)
	{
		if (m_transform)
			m_transform->rotate(angle, x, y, z);
	}

	template<class T>
	bool Geometry::is()
	{
		return false;
	}

	template<class T>
	T *Geometry::as()
	{
		return nullptr;
	}

}