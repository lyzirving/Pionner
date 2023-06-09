#ifndef __DRAW_CMD_GL_H__
#define __DRAW_CMD_GL_H__

#include "render/rhi/DrawCmd.h"

namespace pio
{
	class EntityPart;
	class LightComp;

	class DrawCmdGL : public DrawCmd
	{
	public:
		DrawCmdGL();
		virtual ~DrawCmdGL();

		virtual void drawEntity(RenderEntity &entity, RenderParam &param) override;
		virtual void drawDepth(RenderEntity &entity, RenderParam &param) override;

		virtual void drawGeometry(Geometry &geometry, RenderParam &param) override;
		virtual void drawGeometryDepth(Geometry &geometry, RenderParam &param) override;

		virtual bool drawIdxTriangle(uint32_t idxSize) override;

	private:
		void drawPart(std::shared_ptr<EntityPart> &part, RenderParam &param);
		void drawPartDepth(std::shared_ptr<EntityPart> &part, RenderParam &param);
	};
}

#endif