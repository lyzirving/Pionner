#include "function/render/scene/Layout.h"

#include "function/render/WindowSystem.h"

namespace Pionner
{
	Pionner::Layout::Layout()
		: m_drawPanelInfo()
		, m_windowWidth(0), m_windowHeight(0)
	{
	}

	Layout::~Layout()
	{
	}

	void Layout::initialize(const std::shared_ptr<WindowSystem> &windowSystem)
	{
		m_windowWidth = windowSystem->getWidth();
		m_windowHeight = windowSystem->getHeight();

		// draw panel lies in the center of editor
		m_drawPanelInfo.m_width = uint32_t(m_windowWidth * (2.f / 3.f)) + 1;
		m_drawPanelInfo.m_height = uint32_t(m_windowHeight * (2.f / 3.f)) + 1;
		m_drawPanelInfo.m_top = 0;
		m_drawPanelInfo.m_left = (m_windowWidth - m_drawPanelInfo.m_width) / 2 + 1;

		m_leftPanelInfo.m_left = 0;
		m_leftPanelInfo.m_top = 0;
		m_leftPanelInfo.m_width = (m_windowWidth - m_drawPanelInfo.m_width) / 2;
		m_leftPanelInfo.m_height = m_drawPanelInfo.m_height;

		m_rightPanelInfo.m_left = m_leftPanelInfo.m_left + m_leftPanelInfo.m_width + m_drawPanelInfo.m_width;
		m_rightPanelInfo.m_top = 0;
		m_rightPanelInfo.m_width = m_windowWidth - m_leftPanelInfo.m_width - m_drawPanelInfo.m_width;
		m_rightPanelInfo.m_height = m_windowHeight;

		m_bottomPanelInfo.m_left = 0;
		m_bottomPanelInfo.m_top = m_drawPanelInfo.m_top + m_drawPanelInfo.m_height;
		m_bottomPanelInfo.m_width = m_windowWidth - m_rightPanelInfo.m_width;
		m_bottomPanelInfo.m_height = m_windowHeight - (m_drawPanelInfo.m_height + m_drawPanelInfo.m_top);
	}

	bool Layout::revise(int width, int height)
	{
		if (width != m_windowWidth || height != m_windowHeight)
		{
			m_windowWidth = width;
			m_windowHeight = height;

			m_drawPanelInfo.m_width = uint32_t(m_windowWidth * (2.f / 3.f)) + 1;
			m_drawPanelInfo.m_height = uint32_t(m_windowHeight * (2.f / 3.f)) + 1;
			m_drawPanelInfo.m_top = 0;
			m_drawPanelInfo.m_left = (m_windowWidth - m_drawPanelInfo.m_width) / 2 + 1;

			m_leftPanelInfo.m_left = 0;
			m_leftPanelInfo.m_top = 0;
			m_leftPanelInfo.m_width = (m_windowWidth - m_drawPanelInfo.m_width) / 2;
			m_leftPanelInfo.m_height = m_drawPanelInfo.m_height;

			m_rightPanelInfo.m_left = m_leftPanelInfo.m_left + m_leftPanelInfo.m_width + m_drawPanelInfo.m_width;
			m_rightPanelInfo.m_top = 0;
			m_rightPanelInfo.m_width = m_windowWidth - m_leftPanelInfo.m_width - m_drawPanelInfo.m_width;
			m_rightPanelInfo.m_height = m_windowHeight;

			m_bottomPanelInfo.m_left = 0;
			m_bottomPanelInfo.m_top = m_drawPanelInfo.m_top + m_drawPanelInfo.m_height;
			m_bottomPanelInfo.m_width = m_windowWidth - m_rightPanelInfo.m_width;
			m_bottomPanelInfo.m_height = m_windowHeight - (m_drawPanelInfo.m_height + m_drawPanelInfo.m_top);

			return true;
		}

		return false;
	}
}