#ifndef __PIONNER_UI_LAYER_STACK_H__
#define __PIONNER_UI_LAYER_STACK_H__

#include "Layer.h"

namespace pio
{
	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void pushLayer(Ref<RenderContext>& context, const Ref<Layer>& layer);
		void pushOverlay(Ref<RenderContext>& context, const Ref<Layer>& overlay);

		void popLayer(const Ref<Layer>& layer);
		void popAllLayers();

		void popOverlay(const Ref<Layer>& overlay);
		void popAllOverlays();

		std::vector<Ref<Layer>>::iterator begin() { return m_layers.begin(); }
		std::vector<Ref<Layer>>::iterator end() { return m_layers.end(); }

		std::vector<Ref<Layer>>::reverse_iterator rbegin() { return m_layers.rbegin(); }
		std::vector<Ref<Layer>>::reverse_iterator rend() { return m_layers.rend(); }

		std::vector<Ref<Layer>>::const_iterator begin() const { return m_layers.begin(); }
		std::vector<Ref<Layer>>::const_iterator end() const { return m_layers.end(); }

		std::vector<Ref<Layer>>::const_reverse_iterator rbegin() const { return m_layers.rbegin(); }
		std::vector<Ref<Layer>>::const_reverse_iterator rend() const { return m_layers.rend(); }

	private:
		std::vector<Ref<Layer>> m_layers{};
		uint32_t m_insertIndex{ 0 };
	};

}

#endif