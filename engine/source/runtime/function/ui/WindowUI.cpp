#include <algorithm>

#include "function/ui/WindowUI.h"

namespace Pionner
{
	WindowUI::WindowUI() : m_views(), m_windowWidth(0), m_windowHeight(0)
		, m_needSortView(true)
	{
	}

	WindowUI::~WindowUI()
	{
		auto itr = m_views.begin();
		while (itr != m_views.end())
		{
			(*itr).reset();
			itr = m_views.erase(itr);
		}
	}

	std::shared_ptr<WindowUI> WindowUI::getPtr()
	{
		return shared_from_this();
	}

	ViewLayout WindowUI::getRenderportLayout()
	{
		if (m_views.empty())
		{
			ViewLayout ret{};
			ret.m_width = m_windowWidth;
			ret.m_height = m_windowHeight;
			ret.m_left = ret.m_top = 0;
			return ret;
		}
		sortView();
		// render port should always be rendered firstly
		return m_views[0]->getLayout();
	}

	void WindowUI::resize(int width, int height)
	{
		if (m_windowWidth == width && m_windowHeight == height)
		{
			return;
		}
		m_windowWidth = width;
		m_windowHeight = height;

		sortView();

		for (size_t i = 0; i < m_views.size(); i++)
		{
			if (m_views[i])
				m_views[i]->layout(width, height);
		}
	}

	bool WindowUI::viewSorter(std::shared_ptr<WindowView> &lhs, std::shared_ptr<WindowView> &rhs)
	{
		return lhs->getDrawOrder() < rhs->getDrawOrder();
	}

	void WindowUI::sortView()
	{
		if (m_needSortView)
		{
			if (m_views.size() > 1)
			{
				std::sort(m_views.begin(), m_views.end(), WindowUI::viewSorter);
			}
			m_needSortView = false;
		}
	}

}