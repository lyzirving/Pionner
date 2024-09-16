#ifndef __PIONNER_UI_LAYER_H__
#define __PIONNER_UI_LAYER_H__

#include "UiDef.h"

namespace pio
{
	class Event;	
	class Scene;
	class RenderContext;

	class Layer
	{
	public:
		Layer(const LayoutParams& param, const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void onAttach(Ref<RenderContext>& context) {}
		virtual void onDetach() {}
		virtual bool onEvent(Ref<Event>& event) { return false; }
		// Ensure to be called in render thread		
		virtual void onUpdate(Ref<RenderContext>& context, Ref<Scene>& scene) {}

		const std::string &name() const { return m_name; }
		const LayoutParams &layoutParam() const { return m_layoutParam; }

	protected:
		std::string m_name{};
		LayoutParams m_layoutParam;
	};
}

#endif