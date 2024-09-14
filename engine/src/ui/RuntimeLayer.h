#ifndef __PIONNER_UI_RUNTIME_LAYER_H__
#define __PIONNER_UI_RUNTIME_LAYER_H__

#include "Layer.h"

namespace pio
{
	class KeyPressedEvent;
	class KeyReleasedEvent;
	class MouseMovedEvent;
	class MouseScrolledEvent;
	class MouseButtonPressedEvent;
	class MouseButtonReleasedEvent;

	class RuntimeLayer : public Layer
	{
	public:
		RuntimeLayer(const LayoutParams& param, const std::string& name = "Layer");
		~RuntimeLayer() = default;

		virtual void onAttach(Ref<RenderContext>& context) override;
		virtual void onDetach() override;
		virtual void onUpdate(Ref<RenderContext>& context, Ref<Scene>& scene) override;
		virtual bool onEvent(Ref<Event>& event) override;

	private:
		void onDrawSceneView(Ref<RenderContext>& context, Ref<Scene>& scene);
		void onDrawHierarchyView(Ref<RenderContext>& context, Ref<Scene>& scene);
		void onDrawInspectorView(Ref<RenderContext>& context, Ref<Scene>& scene);

		bool onKeyPress(Ref<KeyPressedEvent>& event);
		bool onKeyRelease(Ref<KeyReleasedEvent>& event);
		bool onMouseMoved(Ref<MouseMovedEvent>& event);
		bool onMouseBtnPress(Ref<MouseButtonPressedEvent>& event);
		bool onMouseBtnRelease(Ref<MouseButtonReleasedEvent>& event);
		bool onMouseScrolled(Ref<MouseScrolledEvent>& event);

	private:
		struct CameraController
		{
			bool InArea{ false }, bRotating{ false };
			bool LeftAltPressed{ false }, RightMouseBtnPressed{ false };
			glm::vec2 Cursor{ 0.f };
		};

	private:
		bool m_resetPosition{ true };
		uint32_t m_selectEntIdx{ InvalidId };
		Ref<Scene> m_activeScene;

		CameraController m_camController;
		LayoutParams m_sceneParam, m_hierarchyParam, m_inspectorParam;
	};
}

#endif