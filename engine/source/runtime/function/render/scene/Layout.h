#ifndef __RENDER_LAYOUT_H__
#define __RENDER_LAYOUT_H__

#include <cstdint>
#include <memory>

namespace Pionner
{
	class WindowSystem;

	struct LayoutInfo
	{
		int32_t  m_left;
		int32_t  m_top;
		uint32_t m_width;
		uint32_t m_height;

		LayoutInfo() : m_left(0), m_top(0), m_width(0), m_height(0) {}
	};

	class Layout
	{
	public:
		Layout();
		~Layout();

		void initialize(const std::shared_ptr<WindowSystem> &windowSystem);

	public:
		LayoutInfo m_drawPanelInfo;
		LayoutInfo m_leftPanelInfo, m_rightPanelInfo;
		LayoutInfo m_bottomPanelInfo;
		uint32_t m_windowWidth, m_windowHeight;
	};
}

#endif