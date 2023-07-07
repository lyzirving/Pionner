#ifndef __SCENE_GRAPH_SHAPE_BOUNDING_BOX_H__
#define __SCENE_GRAPH_SHAPE_BOUNDING_BOX_H__

#include "Shape.h"

namespace pio
{
	namespace gfx
	{
		class BoundingBox : public Shape
		{
		public:
			BoundingBox();
			BoundingBox(const glm::vec3 &min, const glm::vec3 &max);
			virtual ~BoundingBox() = default;

			inline const glm::vec3 &getMin() { return m_min; }
			inline const glm::vec3 &getMax() { return m_max; }

			inline void setMin(const glm::vec3 &min) { m_min = min; }
			inline void setMax(const glm::vec3 &max) { m_max = max; }

		protected:
			glm::vec3 m_min{ 0.f };
			glm::vec3 m_max{ 0.f };
		};
	}
}

#endif	