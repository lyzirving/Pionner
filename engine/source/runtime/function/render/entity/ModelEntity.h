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
		bool coloringWithLight(RenderParam &param, std::shared_ptr<EntityPart> &part, /*out*/std::shared_ptr<Shader> &shader);
		bool coloringWithNormAndLight(RenderParam &param, std::shared_ptr<EntityPart> &part, /*out*/std::shared_ptr<Shader> &shader);
		bool coloringSimple(RenderParam &param, std::shared_ptr<EntityPart> &part, /*out*/std::shared_ptr<Shader> &shader);

		bool dealDirectionLightDepthShader(RenderParam &param, std::shared_ptr<EntityPart> &part, /*out*/std::shared_ptr<Shader> &shader);
		bool dealPointLightDepthShader(RenderParam &param, std::shared_ptr<EntityPart> &part, /*out*/std::shared_ptr<Shader> &shader);
	};
}

#endif