#include "Layer.h"

#include "scene/Registry.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Layer"

namespace pio
{
	Registry *Layer::s_registry = Registry::Get();

	Layer::Layer(const LayoutParams &param, const std::string &name)
		: m_layoutParam(param), m_name(name)
	{
	}
}