#ifndef __TRANSFORM_COMPONENT_H__
#define __TRANSFORM_COMPONENT_H__

#include "function/framework/components/Component.h"

namespace Pionner
{
	class TransformComponent : public Component
	{
	public:
		TransformComponent();
		virtual ~TransformComponent();
		
		virtual void postLoadResource(const std::weak_ptr<GameObject>& parent) override;
		virtual void tick(float delta) override;
	private:

	};
}

#endif