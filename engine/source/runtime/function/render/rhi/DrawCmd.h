#ifndef __DRAW_CMD_H__
#define __DRAW_CMD_H__

#include <memory>

namespace Pionner
{
	class  Geometry;
	class  RenderEntity;
	class  MeshComp;
	struct RenderParam;

	class DrawCmd
	{
	public:
		DrawCmd() {};
		virtual ~DrawCmd() = default;

		virtual void drawEntity(RenderEntity &entity, RenderParam &param) = 0;
		virtual void drawEntityOnly(RenderEntity &entity, RenderParam &param) = 0;
		virtual void drawGeometry(Geometry &geometry, RenderParam &param) = 0;
		virtual void drawDepth(RenderEntity &entity, RenderParam &param) = 0;
	};
}

#endif