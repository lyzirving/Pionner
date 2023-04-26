#ifndef __DRAW_CMD_GL_H__
#define __DRAW_CMD_GL_H__

#include "function/render/cmd/DrawCmd.h"

namespace Pionner
{
	class DrawCmdGL : public DrawCmd
	{
	public:
		DrawCmdGL();
		virtual ~DrawCmdGL();

		virtual void drawEntity(std::shared_ptr<RenderEntity> &entity, RenderParam &param) override;
	};
}

#endif