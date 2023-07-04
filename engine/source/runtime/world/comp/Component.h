#ifndef __PIONNER_COMPONENT_H__
#define __PIONNER_COMPONENT_H__

#include <memory>

#include "ComponentDef.h"

namespace pio
{
	class Component
	{
	public:
		
		Component();
		virtual ~Component() = default;

		inline CompDefine getDefine() { return m_define; }

	protected:
		CompDefine m_define;
	};
}

#endif