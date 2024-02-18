#ifndef __PIONNER_UI_LAYER_RUNTIME_LAYER_H__
#define __PIONNER_UI_LAYER_RUNTIME_LAYER_H__

#include "Layer.h"

namespace pio
{
	class Scene;
	class SceneRenderer;
	class Texture2D;

	class RuntimeLayer : public Layer
	{
	public:
		RuntimeLayer(const LayoutParams &param);
		virtual ~RuntimeLayer();

		virtual void onAttach() override;
		virtual void onDetach() override;		
		virtual bool onEvent(Event &event) override;
		virtual void onRenderDestroy() override;
		virtual void onUpdate(const Timestep &ts) override;
		virtual void onWindowSizeChange(uint32_t width, uint32_t height) override;

	private:
		bool onMouseButtonPressed(Event &event);
		bool onMouseMoved(Event &event);
		bool onMouseButtonReleased(Event &event);

		bool onHandleClick(const glm::vec2 &cursor);
		void onDrawQuad(const AssetHandle &handle, Ref<Texture2D> &composite);

	private:
		Ref<Scene> m_scene;
		Ref<SceneRenderer> m_renderer;
		AssetHandle m_screenQuad{ NullAsset };
	};
}

#endif