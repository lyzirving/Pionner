#ifndef __RENDER_HELPER_H__
#define __RENDER_HELPER_H__

#include <memory>

namespace Pionner
{
	class  RenderEntity;
	class  EntityPart;
	struct RenderParam;

	class RenderHelper
	{
	public:
		static void drawEntity(const std::shared_ptr<RenderEntity> &entity, RenderParam &param);
	private:
		static void drawPart(const std::shared_ptr<EntityPart> &part, RenderParam &param);

		RenderHelper() {}
		~RenderHelper() = default;
	};
}

#endif