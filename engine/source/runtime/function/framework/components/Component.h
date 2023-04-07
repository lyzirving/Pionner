#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <memory>
#include "function/framework/components/ComponentDef.h"

namespace Pionner
{
	class GameObject;

	class Component
	{
	public:
		Component();
		virtual ~Component();

		inline bool isDirty() { return m_dirty; }
		inline void setDirty(bool dirty) { m_dirty = dirty; }
		inline ComponentType getType() const { return m_type; }

		virtual void postLoadResource(const std::weak_ptr<GameObject>& parent);
		virtual void tick(float delta);

	protected:
		std::weak_ptr<GameObject> m_parent;
		ComponentType m_type;
		bool m_dirty;
	};
}

#endif