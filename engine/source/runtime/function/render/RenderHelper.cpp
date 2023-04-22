#include "function/render/RenderHelper.h"

#include "function/render/entity/RenderEntity.h"

namespace Pionner
{
	void RenderHelper::drawEntity(const std::shared_ptr<RenderEntity> &entity, RenderParam &param)
	{
		for (auto &part : entity->m_parts)
		{
			drawPart(part, param);
		}

		if (!entity->m_children.empty())
		{
			auto &children = entity->m_children;
			for (int i = 0; i < children.size(); ++i)
			{
				if (children[i].get())
				{
					drawEntity(children[i], param);
				}
			}
		}
	}

	void RenderHelper::drawPart(const std::shared_ptr<EntityPart> &part, RenderParam &param)
	{
	}
}