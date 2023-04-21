#include <algorithm>

#include "function/render/scene/layer/ModelLayer.h"
#include "function/render/scene/Camera.h"
#include "function/render/scene/Frustum.h"
#include "function/render/entity/RenderEntity.h"

#include "function/render/RenderDef.h"

namespace Pionner
{
	ModelLayer::ModelLayer(const std::shared_ptr<Rhi> &rhi)
		: RenderLayer(rhi)
		, m_entities()
		, m_needSort(false)
	{
	}

	ModelLayer::~ModelLayer()
	{
	}

	void ModelLayer::clear()
	{
		auto itr = m_entities.begin();
		while (itr != m_entities.end())
		{
			(*itr).reset();
			itr = m_entities.erase(itr);
		}
		std::vector<std::shared_ptr<RenderEntity>>().swap(m_entities);
	}

	void ModelLayer::draw(const RenderParam &param)
	{
		if (m_entities.empty())
			return;

		if (m_needSort)
		{
			m_needSort = false;
			std::sort(m_entities.begin(), m_entities.end(), ModelLayer::entitySorter);
		}

		for (auto &entity : m_entities)
		{

		}
	}

	void ModelLayer::addEntities(const std::vector<std::shared_ptr<RenderEntity>> &entities)
	{
		if (!entities.empty())
		{
			m_entities.insert(m_entities.end(), entities.begin(), entities.end());
			m_needSort = true;
		}
	}

	bool ModelLayer::entitySorter(const std::shared_ptr<RenderEntity> &lhs, const std::shared_ptr<RenderEntity> &rhs)
	{
		return lhs->m_order < rhs->m_order;
	}
}