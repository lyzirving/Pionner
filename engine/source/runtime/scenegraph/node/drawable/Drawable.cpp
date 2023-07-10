#include "Drawable.h"

#include "gfx/buffer/Buffer.h"

namespace pio
{
	namespace sgf
	{
		Drawable::Drawable() : Node()
		{
		}

		Drawable::Drawable(const std::string &name) : Node(name)
		{
		}

		Drawable::~Drawable()
		{
			m_vertexBuffer.reset();
			m_indiceBuffer.reset();
		}

		void Drawable::draw(RenderInfo &info)
		{
			drawImplementation(info);
		}

		template <>
		bool Node::is<Drawable>() 
		{
			return m_type == NODE_TYPE_GEOMETRY;
		}

		template <>
		Drawable *Node::as<Drawable>()
		{
			if (is<Drawable>())
			{
				return static_cast<Drawable *>(this);
			}
			return nullptr;
		}
	}
}