#pragma once

#include "ui/imgui/ImGuiWindow.h"

namespace pio
{
	class CameraComponent;
	class MouseScrolledEvent;

	class ScenePanel : public ImGuiWindow
	{
		RTTR_ENABLE(ImGuiWindow)
	public:
		ScenePanel(const std::string& name = "Scene");
		~ScenePanel() = default;

		virtual void OnDraw(const Ref<RenderContext>& context) override;
		virtual bool OnEvent(const Ref<Event>& event) override;

	protected:
		virtual void OnTick(const Ref<RenderContext>& context);

	private:
		bool OnMouseScrolled(const Ref<MouseScrolledEvent>& event);
		void OnDrawFpsText();

	private:
		static glm::vec3 GetCameraHmiMotion(const Ref<CameraComponent>& comp, uint16_t key);

	private:
		glm::vec2 m_CursorPos{ 0.f };
		bool m_bMouseRightDown{ false };
	};
}