#ifndef __UI_DEF_H__
#define __UI_DEF_H__

namespace pio
{
	enum ViewDrawOrder : uint8_t
	{
		ORDER_NONE,
		ORDER_RENDER_PORT,
		ORDER_VIEW_1,
		ORDER_VIEW_2,
		ORDER_VIEW_3,
		ORDER_VIEW_4,
		ORDER_VIEW_5,
		ORDER_VIEW_6
	};

	enum ViewUid : uint8_t
	{
		UID_NONE,
		UID_RENDER_PORT,
		UID_VISUAL_ANGLE,
		UID_LEFT_PANEL,
		UID_RIGHT_PANEL,
		UID_BOTTOM_PANEL
	};

	struct ViewLayout
	{
		int32_t  m_left{ 0 }, m_top{ 0 };
		uint32_t m_width{ 0 }, m_height{ 0 };

		bool contains(int32_t x, int32_t y)
		{
			return (x >= m_left) && (y >= m_top) && (x <= (m_left + m_width)) && (y <= (m_top + m_height));
		}
	};
}

#endif