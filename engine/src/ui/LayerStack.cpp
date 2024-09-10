#include "LayerStack.h"

namespace pio
{
	LayerStack::~LayerStack()
	{
		auto it = m_layers.begin();
		while(it != m_layers.end())
		{
			if((*it).get())
			{
				(*it)->onDetach();
			}
			(*it).reset();
			it = m_layers.erase(it);
		}
	}

	void LayerStack::pushLayer(Ref<RenderContext>& context, const Ref<Layer>& layer)
	{
		m_layers.emplace(m_layers.begin() + m_insertIndex, layer);
		m_insertIndex++;
		layer->onAttach(context);
	}

	void LayerStack::pushOverlay(Ref<RenderContext>& context, const Ref<Layer>& overlay)
	{
		m_layers.emplace_back(overlay);
		overlay->onAttach(context);
	}

	void LayerStack::popLayer(const Ref<Layer>& layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.begin() + m_insertIndex, layer);
		if(it != m_layers.begin() + m_insertIndex)
		{
			if((*it))
				(*it)->onDetach();
			(*it).reset();
			m_layers.erase(it);
			m_insertIndex--;
		}
	}

	void LayerStack::popAllLayers()
	{
		auto it = m_layers.begin();
		auto end = m_layers.begin() + m_insertIndex;

		while(it != end)
		{
			if((*it))
				(*it)->onDetach();
			(*it).reset();
			it++;
			m_insertIndex--;
		}
		m_layers.erase(m_layers.begin(), end);
	}

	void LayerStack::popOverlay(const Ref<Layer>& overlay)
	{
		auto it = std::find(m_layers.begin() + m_insertIndex, m_layers.end(), overlay);
		if(it != m_layers.end())
		{
			if((*it))
				(*it)->onDetach();
			(*it).reset();
			m_layers.erase(it);
		}
	}

	void LayerStack::popAllOverlays()
	{
		auto it = m_layers.begin() + m_insertIndex;
		auto end = m_layers.end();

		while(it != end)
		{
			if((*it))
				(*it)->onDetach();
			(*it).reset();
			it++;
		}
		m_layers.erase(m_layers.begin(), end);
	}

}
