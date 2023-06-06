#include "function/render/geo/Cylinder.h"

#include "function/framework/comp/MeshComp.h"

#include "function/render/RenderDef.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Cylinder"

namespace Pionner
{
	Cylinder::Cylinder() : GeoObj("cylinder")
	{
		m_meshComp = std::shared_ptr<MeshComp>(new MeshComp);
	}

	Cylinder::~Cylinder()
	{
	}

	void Cylinder::draw(RenderParam &param)
	{
	}

	void Cylinder::initialize(RenderParam &param)
	{
	}
}