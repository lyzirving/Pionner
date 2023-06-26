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

		virtual bool dealShader(RenderParam &param, std::shared_ptr<EntityPart> &part, /*out*/std::shared_ptr<Shader> &shader) override;

		virtual bool dealDepthShader(RenderParam &param, std::shared_ptr<EntityPart> &part, /*out*/std::shared_ptr<Shader> &shader) override;

	private:
		bool shadeWithNormAndLight(RenderParam &param, std::shared_ptr<EntityPart> &part, /*out*/std::shared_ptr<Shader> &shader);
	};
}

#endif