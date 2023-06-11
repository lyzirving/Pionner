#ifndef __DRAW_CMD_GL_H__
#define __DRAW_CMD_GL_H__

#include "function/render/rhi/DrawCmd.h"

namespace Pionner
{
	class EntityPart;
	class LightComp;

	class DrawCmdGL : public DrawCmd
	{
	public:
		DrawCmdGL();
		virtual ~DrawCmdGL();

		virtual void drawEntity(std::shared_ptr<RenderEntity> &entity, RenderParam &param) override;
		virtual void drawGeometry(Geometry &geometry, RenderParam &param) override;

	private:
		void drawPart(std::shared_ptr<EntityPart> &part, RenderParam &param);

		void drawLightCaster(std::shared_ptr<RenderEntity> &entity, RenderParam &param);
		void drawLightCasterPart(std::shared_ptr<EntityPart> &part, RenderParam &param);
	};
}

#endif