#ifndef __PIONNER_UI_VIEW_H__
#define __PIONNER_UI_VIEW_H__

#include "UiDef.h"
#include "scene/SceneDef.h"

namespace pio
{
	class Geometry2D;
	struct TextureSpecification;

	enum ViewCtlStatus : uint8_t
	{
		ViewCtlStatus_Normal = 0, ViewCtlStatus_Disnabled, ViewCtlStatus_Pressed, ViewCtlStatus_Selected,
		ViewCtlStatus_Num
	};

	class View
	{
	public:
		View(const std::string &name) : m_name(name) {}
		virtual ~View() = default;

		virtual void upload();

		void invalidate(bool val = true) { m_invalidate = val; }

		void setTexture(const AssetHandle &h) { m_texture = h; }
		void setStatus(ViewCtlStatus s) { m_ctlStatus = s; }
		void setPosition(uint32_t l, uint32_t t, uint32_t width, uint32_t height) 
		{ 
			LayoutRect rect(l, t, l + width, t + height); 
			if (m_rect != rect)
			{
				m_rect = rect;
				invalidate();
			}
		}

		void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			LayoutViewport vp(x, y, width, height);
			if (m_viewport != vp)
			{
				m_viewport = vp;
				invalidate();
			}
		}

		bool needUpdate() const { return m_invalidate; }
		bool isSelected() const { return m_ctlStatus == ViewCtlStatus_Selected; }
		bool isPressed() const { return m_ctlStatus == ViewCtlStatus_Pressed; }
		bool isDisnabled() const { return m_ctlStatus == ViewCtlStatus_Disnabled; }

		AssetHandle getMesh() const;
		AssetHandle getTexture() const { return m_texture; }		

	protected:
		std::string m_name;
		LayoutRect m_rect{};
		LayoutViewport m_viewport{};

		ViewCtlStatus m_ctlStatus{ ViewCtlStatus_Normal };
		bool m_invalidate{ false };

		AssetHandle m_texture{ NullAsset };
		Ref<Geometry2D> m_quad;
	};
}

#endif