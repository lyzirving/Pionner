#include "function/render/swap/SwapContext.h"
#include "function/render/entity/RenderEntity.h"

namespace Pionner
{
	SwapData::SwapData() : m_changeFlag(), m_entities()
	{
	}

	void SwapData::addEntities(std::vector<std::shared_ptr<RenderEntity>> &entities)
	{
		m_changeFlag.set(SOURCE_RENDER_ENTITY);
		m_entities.insert(m_entities.end(), entities.begin(), entities.end());
	}

	void SwapData::clear()
	{
		clearEntity();
		m_changeFlag.reset();
	}

	void SwapData::clearEntity()
	{
		if (!m_entities.empty())
		{
			auto itr = m_entities.begin();
			while (itr != m_entities.end())
			{
				(*itr).reset();
				itr = m_entities.erase(itr);
			}
			std::vector<std::shared_ptr<RenderEntity>>().swap(m_entities);
		}
	}

	SwapContext::SwapContext() : m_data()
		, m_logicInd(TYPE_LOGIC)
		, m_renderInd(TYPE_RENDER)
	{
		logicData().m_changeFlag.reset();
		renderData().m_changeFlag.reset();
	}

	SwapContext::~SwapContext()
	{
		logicData().clear();
		renderData().clear();
	}

	bool SwapContext::needSwap()
	{
		SwapData &logic = logicData();
		for (uint8_t src = SOURCE_RENDER_ENTITY; src < SWAP_SOURCE_COUNT; ++src)
		{
			if (logic.m_changeFlag.test(src))
				return true;
		}
		return false;
	}

	void SwapContext::swapData()
	{
		SwapData &render = renderData();

		render.m_changeFlag.reset();
		render.clearEntity();

		std::swap(m_logicInd, m_renderInd);
	}

	SwapData &SwapContext::logicData()
	{
		return m_data[m_logicInd];
	}

	SwapData &SwapContext::renderData()
	{
		return m_data[m_renderInd];
	}
}