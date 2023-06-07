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