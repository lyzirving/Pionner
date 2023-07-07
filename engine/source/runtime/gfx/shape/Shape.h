#ifndef __SCENE_GRAPH_SHAPE_H__
#define __SCENE_GRAPH_SHAPE_H__

#include <glm/glm.hpp>

namespace pio
{
	namespace gfx
	{
		enum ShapeType : uint8_t
		{
			SHAPE_BOUNDING_BOX,
			SHAPE_CNT
		};

		/** Base class for all shape types.
		  * Shapes are used to either for culling and collision detection or
		  * to define the geometric shape of procedurally generate Geometry.
		*/
		class Shape
		{
		public:
			Shape() {}
			Shape(ShapeType type) : m_type(type) {}
			virtual ~Shape() = default;

		protected:
			ShapeType m_type{ SHAPE_CNT };
		};
	}
}

#endif	