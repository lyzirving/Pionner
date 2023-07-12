#ifndef __SCENE_GRAPH_SHAPE_H__
#define __SCENE_GRAPH_SHAPE_H__

#include <glm/glm.hpp>

#include "gfx/GfxDef.h"

namespace pio
{
	namespace gfx
	{
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