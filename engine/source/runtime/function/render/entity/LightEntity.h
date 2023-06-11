#ifndef __RENDER_ENTITY_LIGHT_H__
#define __RENDER_ENTITY_LIGHT_H__

#include "RenderEntity.h"

namespace Pionner
{
	class LightEntity : public RenderEntity
	{
	public:
		LightEntity();
		virtual ~LightEntity();

		virtual bool dealShader(RenderParam &param, std::shared_ptr<EntityPart> &part,
								/*out*/std::shared_ptr<Shader> &shader,
								/*out*/std::shared_ptr<GfxBuffer> &texture) override;
		virtual std::shared_ptr<RenderEntity> makeEmptyEntity() override;

	};
}

#endif