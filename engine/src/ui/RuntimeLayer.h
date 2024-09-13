#ifndef __PIONNER_UI_RUNTIME_LAYER_H__
#define __PIONNER_UI_RUNTIME_LAYER_H__

#include "Layer.h"

namespace pio
{
	class RuntimeLayer : public Layer
	{
	public:
		RuntimeLayer(const LayoutParams& param, const std::string& name = "Layer");
		~RuntimeLayer() = default;

		virtual void onAttach(Ref<RenderContext>& context) override;
		virtual void onDetach() override;
		virtual void onUpdate(Ref<RenderContext>& context, Ref<Scene>& scene, Ref<RenderTarget>& target) override;
		virtual bool onEvent(Ref<Event>& event) override;

	private:
		void onDrawSceneView(Ref<RenderContext>& context, Ref<Scene>& scene, Ref<RenderTarget>& target, const LayoutRatio& layout, bool firstTime);
		void onDrawHierarchyView(Ref<RenderContext>& context, Ref<Scene>& scene, Ref<RenderTarget>& target, const LayoutRatio& layout, bool firstTime);
		void onDrawInspectorView(Ref<RenderContext>& context, Ref<Scene>& scene, Ref<RenderTarget>& target, const LayoutRatio& layout, bool firstTime);

	private:
		bool m_firstTimeShow{ true };
		uint32_t m_selectEntIdx{ InvalidId };
	};
}

#endif