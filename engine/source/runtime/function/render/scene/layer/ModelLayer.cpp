#include "function/render/scene/layer/ModelLayer.h"
#include "function/render/entity/RenderEntity.h"

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

	void ModelLayer::draw()
	{
		if (m_needSort)
		{
			m_needSort = false;
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
}