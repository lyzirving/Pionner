#include "IndirectLightComponent.h"

#include "gfx/renderer/IndirectLight.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "IndirectLightComponent"

namespace pio
{
	void IndirectLightComponent::OnAttach()
	{
		m_IndirectLight = CreateRef<IndirectLight>(m_Context.lock(), "IndirectLight");
	}

	void IndirectLightComponent::OnTick()
	{
		m_IndirectLight->OnTick();
	}
}