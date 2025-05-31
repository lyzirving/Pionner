#include "Component.h"

#include "scene/node/Node.h"

namespace pio
{
	bool Component::HaveParentHost() const
	{
		return !m_Host.expired() && m_Host.lock()->IsChild();
	}
}