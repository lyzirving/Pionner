#ifndef __PIONNER_SCENE_NODE_MOVABLE_NODE_H__
#define __PIONNER_SCENE_NODE_MOVABLE_NODE_H__

#include "Node.h"

namespace pio
{
	class MovableNode : public Node
	{
		PIO_NODE_DECLARE_CONSTRUCTOR(MovableNode)
	public:
		virtual ~MovableNode();
		virtual void onInit() override;

		void addTranslation(const glm::vec3& delta);
		void addRotation(const glm::vec3& delta);

		void setPosition(const glm::vec3& pos);
		void setRotation(const glm::vec3& euler);
		void setScale(const glm::vec3& scale);
	};
}

#endif