#ifndef __PIONNER_LIGHT_COMP_H__
#define __PIONNER_LIGHT_COMP_H__

#include "function/framework/comp/Comp.h"
#include "function/framework/world/light/Light.h"

#include "function/render/entity/RenderEntity.h"

namespace Pionner
{
	class LightComp : public Comp
	{
	public:
		LightComp();
		virtual ~LightComp();

	public:
		std::shared_ptr<Light>        m_light;
		std::shared_ptr<RenderEntity> m_entity;
	};
}

#endif