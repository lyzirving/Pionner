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
	}
}