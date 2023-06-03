#ifndef __UI_DEF_H__
#define __UI_DEF_H__

namespace Pionner
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
		UID_LEFT_PANEL,
		UID_RIGHT_PANEL,
		UID_BOTTOM_PANEL
	};

	struct ViewLayout
	{
		int32_t  m_left;
		int32_t  m_top;
		uint32_t m_width;
		uint32_t m_height;

		ViewLayout() : m_left(0), m_top(0), m_width(0), m_height(0) {}
	};
}

#endif