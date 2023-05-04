#ifndef __PIONNER_RENDER_COMP_H__
#define __PIONNER_RENDER_COMP_H__

#include "function/framework/comp/Comp.h"
#include "function/render/entity/RenderEntity.h"

namespace Pionner
{
	class RenderComp : public Comp
	{
	public:
		RenderComp();
		virtual ~RenderComp();

	public:
		std::shared_ptr<RenderEntity> m_entity;
	};
}

#endif