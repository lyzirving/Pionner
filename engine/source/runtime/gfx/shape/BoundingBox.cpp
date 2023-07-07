#include "BoundingBox.h"

namespace pio
{
	namespace gfx
	{
		BoundingBox::BoundingBox() : Shape(SHAPE_BOUNDING_BOX)
		{
		}

		BoundingBox::BoundingBox(const glm::vec3 &min, const glm::vec3 &max) 
			: Shape(SHAPE_BOUNDING_BOX), m_min(min), m_max(max)
		{
		}
	}
}