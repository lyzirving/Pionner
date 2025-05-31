#pragma once

#include "ImGuiWidget.h"

namespace pio
{
	class ImGuiWindow : public ImGuiWidget
	{
		RTTR_ENABLE(ImGuiWidget)
	public:
		ImGuiWindow(const std::string& name = "");
		virtual ~ImGuiWindow();

		virtual void OnBeforeDraw(const Ref<RenderContext>& context) override;
		virtual void OnPostDraw(const Ref<RenderContext>& context) override;

		void EnableMove(bool val);
		void EnableDocking(bool val);
		void EnableResize(bool val);
		void EnableTitle(bool val);

	public:
		static const int32_t k_CommonFlags;

	private:
		static int32_t k_InstNum;

	protected:
		int32_t m_WindowFlags{ 0 };
	};
}