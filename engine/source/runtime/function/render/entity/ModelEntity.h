#ifndef __RENDER_ENTITY_MODEL_H__
#define __RENDER_ENTITY_MODEL_H__

#include "RenderEntity.h"

namespace Pionner
{
	class ModelEntity : public RenderEntity
	{
	public:
		ModelEntity(RenderMode mode = RENDER_MODE_RENDERED_DISPLAY);
		virtual ~ModelEntity();

		virtual bool dealShader(RenderParam &param, std::shared_ptr<EntityPart> &part, /*out*/std::shared_ptr<Shader> &shader) override;

		virtual bool dealDepthShader(RenderParam &param, std::shared_ptr<EntityPart> &part, /*out*/std::shared_ptr<Shader> &shader) override;

	private:
		bool coloringRenderDisplay(RenderParam &param, std::shared_ptr<EntityPart> &part, /*out*/std::shared_ptr<Shader> &shader);
		bool coloringMaterialDisplay(RenderParam &param, std::shared_ptr<EntityPart> &part, /*out*/std::shared_ptr<Shader> &shader);

		bool coloringWithLight(RenderParam &param, std::shared_ptr<EntityPart> &part, /*out*/std::shared_ptr<Shader> &shader);
		bool coloringWithNormAndLight(RenderParam &param, std::shared_ptr<EntityPart> &part, /*out*/std::shared_ptr<Shader> &shader);

		bool dealDirectionLightDepthShader(RenderParam &param, std::shared_ptr<EntityPart> &part, /*out*/std::shared_ptr<Shader> &shader);
		bool dealPointLightDepthShader(RenderParam &param, std::shared_ptr<EntityPart> &part, /*out*/std::shared_ptr<Shader> &shader);
	};
}

#endif