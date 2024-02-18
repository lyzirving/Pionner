#include "LayerManager.h"

#include "window/event/AppEvent.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "LayerManager"

namespace pio
{
	LayerManager::~LayerManager()
	{
		popAll();
	}

	bool LayerManager::onEvent(Event &event)
	{
		for (auto it = m_stack.end(); it != m_stack.begin(); )
		{
			(*--it)->onEvent(event);
			PIO_CHECK_EVT_HANDLE_AND_BREAK(event);
		}
		return event.Handled;
	}

	void LayerManager::onRenderDestroy()
	{
		for (auto layer : m_stack)
		{
			if (layer)
				layer->onRenderDestroy();
		}
	}

	void LayerManager::onUpdate(const Timestep &ts)
	{
		for (auto layer : m_stack)
		{
			if (layer)
				layer->onUpdate(ts);
		}
	}

	bool LayerManager::onWindowResize(Event &event)
	{
		auto *p = event.as<WindowResizeEvent>();
		if (p)
		{
			for (auto layer : m_stack)
			{
				if (layer)
					layer->onWindowSizeChange(p->getWidth(), p->getHeight());
			}
			return true;
		}
		return false;
	}
}