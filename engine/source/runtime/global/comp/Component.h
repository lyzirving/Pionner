#ifndef __PIONNER_COMPONENT_H__
#define __PIONNER_COMPONENT_H__

#include <memory>

namespace pio
{
	enum CompDefine : uint8_t
	{
		CMP_TRANSFORM,
		CMP_LIGHT,
		CMP_MATERIAL,
		CMP_CNT
	};

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