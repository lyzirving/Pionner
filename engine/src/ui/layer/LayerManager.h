#ifndef __PIONNER_UI_LAYER_LAYER_MANAGER_H__
#define __PIONNER_UI_LAYER_LAYER_MANAGER_H__

#include "LayerStack.h"

namespace pio
{
	class Event;

	class LayerManager
	{
	public:
		LayerManager() {}
		~LayerManager();

		bool onEvent(Event &event);
		void onRenderDestroy();
		void onUpdate(const Timestep &ts);
		void onUpdateUI(const Timestep &ts);
		bool onWindowResize(Event &event);

	public:
		inline void pushLayer(const Ref<Layer> &layer) { if (layer) m_stack.pushLayer(layer); }
		inline void pushOverlay(const Ref<Layer> &layer) { if (layer) m_stack.pushOverlay(layer); }
		inline void popLayer(const Ref<Layer> &layer) { if (layer) m_stack.popLayer(layer); }
		inline void popOverlay(const Ref<Layer> &layer) { if (layer) m_stack.popOverlay(layer); }
		inline void popAll() { m_stack.popAllLayers(); m_stack.popAllOverlays(); }

	private:
		LayerStack m_stack{};
	};
}

#endif