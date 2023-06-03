#include <algorithm>

#include "function/ui/WindowUI.h"

namespace Pionner
{
	WindowUI::WindowUI() : m_viewMap(), m_viewArray()
		, m_windowWidth(0), m_windowHeight(0), m_needSortView(true)
	{
	}

	WindowUI::~WindowUI()
	{
		auto itr = m_viewArray.begin();
		while (itr != m_viewArray.end())
		{
			(*itr).second.reset();
			itr = m_viewArray.erase(itr);
		}

		auto itr1 = m_viewMap.begin();
		while (itr1 != m_viewMap.end())
		{
			itr1->second.reset();
			itr1 = m_viewMap.erase(itr1);
		}
	}

	void WindowUI::addView(const std::shared_ptr<WindowView> &view)
	{
		if (contain(view))
		{
			return;
		}
		m_needSortView = true;
		m_viewMap.insert(std::make_pair(view->getUid(), view));
	}

	bool WindowUI::contain(const std::shared_ptr<WindowView> &view)
	{
		return m_viewMap.find(view->getUid()) != m_viewMap.end();
	}

	void WindowUI::layout()
	{
		sortView();

		for (size_t i = 0; i < m_viewArray.size(); i++)
		{
			if (m_viewArray[i].second)
				m_viewArray[i].second->layout(m_windowWidth, m_windowHeight);
		}
	}

	std::shared_ptr<WindowUI> WindowUI::getPtr()
	{
		return shared_from_this();
	}

	std::shared_ptr<WindowView> WindowUI::getView(uint8_t uid)
	{
		auto itr = m_viewMap.find(uid);
		if (itr != m_viewMap.end())
		{
			return itr->second;
		}
		else
		{
			return std::shared_ptr<WindowView>();
		}
	}

	void WindowUI::resize(int width, int height)
	{
		if (m_windowWidth == width && m_windowHeight == height)
		{
			return;
		}
		m_windowWidth = width;
		m_windowHeight = height;

		layout();
	}

	bool WindowUI::pairSorter(const ViewItem &lhs, const ViewItem &rhs)
	{
		return lhs.second->getDrawOrder() < rhs.second->getDrawOrder();
	}

	void WindowUI::sortView()
	{
		if (m_needSortView)
		{
			m_viewArray.clear();
			m_viewArray.assign(m_viewMap.begin(), m_viewMap.end());
			std::sort(m_viewArray.begin(), m_viewArray.end(), pairSorter);
			m_needSortView = false;
		}
	}

}