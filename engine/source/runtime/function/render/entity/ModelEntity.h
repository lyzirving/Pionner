#ifndef __RENDER_ENTITY_MODEL_H__
#define __RENDER_ENTITY_MODEL_H__

#include "RenderEntity.h"

namespace Pionner
{
	class ModelEntity : public RenderEntity
	{
	public:
		ModelEntity();
		virtual ~ModelEntity();

		virtual bool dealShader(RenderParam &param, std::shared_ptr<EntityPart> &part,
								/*out*/std::shared_ptr<Shader> &shader,
								/*out*/std::shared_ptr<GfxBuffer> &texture) override;
		virtual std::shared_ptr<RenderEntity> makeEmptyEntity() override;

	};
}

#endif