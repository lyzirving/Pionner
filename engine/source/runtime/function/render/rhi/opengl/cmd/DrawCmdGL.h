#ifndef __DRAW_CMD_GL_H__
#define __DRAW_CMD_GL_H__

#include "function/render/rhi/DrawCmd.h"

namespace Pionner
{
	class EntityPart;

	class DrawCmdGL : public DrawCmd
	{
	public:
		DrawCmdGL();
		virtual ~DrawCmdGL();

		virtual void drawEntity(std::shared_ptr<RenderEntity> &entity, RenderParam &param) override;
		virtual void drawInfiniteGrid(std::shared_ptr<MeshComp> &mesh, RenderParam &param) override;

	private:
		void drawPart(std::shared_ptr<EntityPart> &part, RenderParam &param);
	};
}

#endif