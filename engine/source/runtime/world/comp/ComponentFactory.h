#ifndef __PIONNER_COMPONENT_FACTORY_H__
#define __PIONNER_COMPONENT_FACTORY_H__

#include "TransformComponent.h"
#include "MaterialComponent.h"

namespace pio
{
	class ComponentFactory
	{
	public:

		template<class T>
		static  std::shared_ptr<Component> create();

	private:
		ComponentFactory() {};
		~ComponentFactory() = default;
	};

	template<class T>
	std::shared_ptr<Component> ComponentFactory::create()
	{
		std::shared_ptr<Component> cmp{ nullptr };
		if (std::is_base_of<Component, T>())
		{
			cmp = std::shared_ptr<Component>(new T);
		}
		return cmp;
	}
}

#endif