#ifndef __PIONNER_UI_VIEW_H__
#define __PIONNER_UI_VIEW_H__

#include "UiDef.h"

namespace pio
{
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class Texture2D;
	struct TextureSpecification;

	enum ViewCtlStatus : uint8_t
	{
		ViewCtlStatus_Normal = 0, ViewCtlStatus_Disnabled, ViewCtlStatus_Pressed, ViewCtlStatus_Selected
	};

	class View
	{
	public:
		View(const std::string &name) : m_name(name) {}
		virtual ~View() = default;

		virtual void upload() {}

		void invalidate(bool val = true) { m_invalidate = val; }
		void setStatus(ViewCtlStatus s) { m_ctlStatus = s; }
		void setPosition(uint32_t l, uint32_t t, uint32_t r, uint32_t b) 
		{ 
			LayoutRect rect(l, t, r, b); 
			if (rect != m_rect)
			{
				m_rect = rect;
				invalidate();
			}
		}

		bool needUpdate() const { return m_invalidate; }
		bool isSelected() const { return m_ctlStatus == ViewCtlStatus_Selected; }
		bool isPressed() const { return m_ctlStatus == ViewCtlStatus_Pressed; }
		bool isDisnabled() const { return m_ctlStatus == ViewCtlStatus_Disnabled; }

	protected:
		std::string m_name;
		LayoutRect m_rect{};

		ViewCtlStatus m_ctlStatus{ ViewCtlStatus_Normal };
		bool m_invalidate{ true };

		Ref<VertexArray> VertexArray{};
		Ref<VertexBuffer> VertexBuffer{};
		Ref<IndexBuffer> IndexBuffer{};
	};

	class IconView : public View
	{
	public:
		IconView(const std::string &name) : View(name) {}
		virtual ~IconView() = default;

		virtual void upload() override;

	protected:
		AssetHandle m_quadMesh{ NullAsset };
		Ref<Texture2D> m_texture;
	};
}

#endif