#ifndef __DRAW_CMD_H__
#define __DRAW_CMD_H__

#include <memory>

namespace Pionner
{
	class RenderEntity;
	struct RenderParam;

	class DrawCmd
	{
	public:
		DrawCmd() {};
		virtual ~DrawCmd() = default;

		virtual void drawEntity(std::shared_ptr<RenderEntity> &entity, RenderParam &param) = 0;
	};
}

#endif