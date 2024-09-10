#include "LayerMgr.h"

#include "event/Event.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "LayerManager"

namespace pio
{
	LayerMgr::LayerMgr()
	{
	}

	LayerMgr::~LayerMgr()
	{
		popAll();
	}

	void LayerMgr::onUpdate(Ref<RenderContext>& context, Ref<Scene>& scene, Ref<RenderTarget>& target)
	{
		for(auto layer : m_stack)
		{
			if(layer)
				layer->onUpdate(context, scene, target);
		}
	}

	bool LayerMgr::onEvent(Ref<Event>& event)
	{
		for(auto it = m_stack.end(); it != m_stack.begin(); )
		{
			(*--it)->onEvent(event);
			PIO_CHECK_EVT_HANDLE_AND_BREAK(event);
		}
		return event->Handled;
	}
}