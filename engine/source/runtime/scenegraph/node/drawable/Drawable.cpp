#include "Drawable.h"

#include "gfx/buffer/Buffer.h"

namespace pio
{
	namespace sgf
	{
		Drawable::Drawable() : Node(), m_vertexBuffer(nullptr), m_indiceBuffer(nullptr)
		{
		}

		Drawable::Drawable(const std::string &name) : Node(name), m_vertexBuffer(nullptr), m_indiceBuffer(nullptr)
		{
		}

		Drawable::~Drawable()
		{
			m_vertexBuffer.reset();
			m_indiceBuffer.reset();
		}

		void Drawable::draw(RenderInfo &info)
		{
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