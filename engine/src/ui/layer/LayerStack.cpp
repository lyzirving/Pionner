#include "LayerStack.h"

namespace pio
{
	LayerStack::~LayerStack()
	{
		auto it = m_layers.begin();
		while (it != m_layers.end())
		{
			if ((*it).get())
			{
				(*it)->onDetach();
			}
			(*it).reset();
			it = m_layers.erase(it);
		}
	}

	void LayerStack::pushLayer(const Ref<Layer> &layer)
	{
		m_layers.emplace(m_layers.begin() + m_layerInsertIndex, layer);
		m_layerInsertIndex++;
		layer->onAttach();
	}

	void LayerStack::pushOverlay(const Ref<Layer> &overlay)
	{
		m_layers.emplace_back(overlay);
		overlay->onAttach();
	}

	void LayerStack::popLayer(const Ref<Layer> &layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.begin() + m_layerInsertIndex, layer);
		if (it != m_layers.begin() + m_layerInsertIndex)
		{
			if ((*it))
				(*it)->onDetach();
			(*it).reset();
			m_layers.erase(it);
			m_layerInsertIndex--;
		}
	}

	void LayerStack::popAllLayers()
	{
		auto it = m_layers.begin();
		auto end = m_layers.begin() + m_layerInsertIndex;

		while (it != end)
		{
			if ((*it))
				(*it)->onDetach();
			(*it).reset();
			it++;
			m_layerInsertIndex--;
		}
		m_layers.erase(m_layers.begin(), end);
	}

	void LayerStack::popOverlay(const Ref<Layer> &overlay)
	{
		auto it = std::find(m_layers.begin() + m_layerInsertIndex, m_layers.end(), overlay);
		if (it != m_layers.end())
		{
			if ((*it))
				(*it)->onDetach();
			(*it).reset();
			m_layers.erase(it);
		}
	}

	void LayerStack::popAllOverlays()
	{
		auto it = m_layers.begin() + m_layerInsertIndex;
		auto end = m_layers.end();

		while (it != end)
		{
			if ((*it))
				(*it)->onDetach();
			(*it).reset();
			it++;
		}
		m_layers.erase(m_layers.begin(), end);
	}

}
