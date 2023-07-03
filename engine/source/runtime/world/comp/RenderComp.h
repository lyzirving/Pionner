#ifndef __PIONNER_RENDER_COMP_H__
#define __PIONNER_RENDER_COMP_H__

#include "Comp.h"
#include "render/entity/RenderEntity.h"

namespace pio
{
	class RenderComp : public Comp
	{
	public:
		RenderComp();
		virtual ~RenderComp();

	public:
		std::shared_ptr<RenderEntity>  m_entity;
	};
}

#endif