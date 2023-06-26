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
								/*out*/std::shared_ptr<Shader> &shader) override;

		virtual bool dealDepthShader(RenderParam &param, std::shared_ptr<EntityPart> &part,
									 /*out*/std::shared_ptr<Shader> &shader) override;
	};
}

#endif