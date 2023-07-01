#include "LightComp.h"

#include "function/render/entity/RenderEntity.h"

namespace Pionner
{
	LightComp::LightComp() : Comp()
		, m_pos(0.f), m_dir(0.f)
		, m_ka(1.f), m_kd(1.f), m_ks(1.f)
		, m_ia(0.2f), m_id(0.6f), m_is(0.7f)
		, m_shininess(64.f)
		, m_obj(nullptr)
		, m_type(LIGHT_TYPE_DIRECTIONAL)
	{
	}

	LightComp::~LightComp()
	{
		m_obj.reset();
	}
}