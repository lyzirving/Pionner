#ifndef __PIONNER_UI_LAYER_MGR_H__
#define __PIONNER_UI_LAYER_MGR_H__

#include "LayerStack.h"

namespace pio
{
	class RenderContext;
	class RenderTarget;
	class Scene;
	class Event;

	class LayerMgr
	{
	public:
		LayerMgr();
		~LayerMgr();

		void onUpdate(Ref<RenderContext>& context, Ref<Scene>& scene, Ref<RenderTarget>& target);
		bool onEvent(Ref<Event>& event);

		void pushLayer(Ref<RenderContext>& context, const Ref<Layer> &layer) { if (layer) m_stack.pushLayer(context, layer); }
		void pushOverlay(Ref<RenderContext>& context, const Ref<Layer> &layer) { if (layer) m_stack.pushOverlay(context, layer); }
		void popLayer(const Ref<Layer> &layer) { if (layer) m_stack.popLayer(layer); }
		void popOverlay(const Ref<Layer> &layer) { if (layer) m_stack.popOverlay(layer); }
		void popAll() { m_stack.popAllLayers(); m_stack.popAllOverlays(); }

	private:
		LayerStack m_stack;
	};
}

#endif