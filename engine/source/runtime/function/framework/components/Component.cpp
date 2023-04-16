#include "function/framework/components/Component.h"
#include "function/framework/object/GameObject.h"

namespace Pionner
{
	Component::Component()
		: m_parent()
		, m_dirty(false)
		, m_type(CMP_NONE)
	{
	}

	Component::~Component()
	{
		m_parent.reset();
	}

	void Component::postLoadResource(const std::weak_ptr<GameObject>& parent)
	{
		// replaces the managed object with the one managed by parent.
		// The object is shared with parent.
		m_parent = parent;
	}

	void Component::tick(float delta)
	{
	}

	void Component::tickLogicEvent(float delta, EventType type, const std::shared_ptr<EventArg>& arg)
	{
	}

}