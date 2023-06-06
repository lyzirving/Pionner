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

		virtual void drawEntity(std::shared_ptr<RenderEntity> &entity, RenderParam &param) = 0;
		virtual void drawCircle(std::shared_ptr<MeshComp> &mesh, RenderParam &param) = 0;
		virtual void drawInfiniteGrid(std::shared_ptr<MeshComp> &mesh, RenderParam &param) = 0;
		virtual void drawColorGeometry(Geometry &geometry, RenderParam &param) = 0;
	};
}

#endif