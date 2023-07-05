#include "Geometry.h"

namespace pio
{
	namespace sgf
	{
		Geometry::Geometry() : Drawable(), m_vertexs(), m_indices()
		{
		}

		Geometry::Geometry(const std::string &name) : Drawable(name), m_vertexs(), m_indices()
		{
		}

		Geometry::~Geometry()
		{
		}

		void Geometry::drawImplementation(RenderInfo &info)
		{
		}
	}
}