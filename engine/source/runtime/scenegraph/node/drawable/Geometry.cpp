#include "Geometry.h"

namespace pio
{
	namespace sgf
	{
		Geometry::Geometry() : Drawable(), m_vertexs(), m_indices()
		{
			m_type = NODE_TYPE_GEOMETRY;
		}

		Geometry::Geometry(const std::string &name) : Drawable(name), m_vertexs(), m_indices()
		{
			m_type = NODE_TYPE_GEOMETRY;
		}

		Geometry::~Geometry()
		{
		}

		void Geometry::setVertexArray(const std::vector<Vertex> &vertexArray)
		{
			m_vertexs.clear();
			if (!vertexArray.empty())
			{
				m_vertexs.assign(vertexArray.begin(), vertexArray.end());
			}
		}

		void Geometry::setIndiceArray(const std::vector<uint32_t> &indiceArray)
		{
			m_indices.clear();
			if (!indiceArray.empty())
			{
				m_indices.assign(indiceArray.begin(), indiceArray.end());
			}
		}

		void Geometry::drawImplementation(RenderInfo &info)
		{
		}

		template <>
		bool Node::is<Geometry>() const
		{
			return m_type == NODE_TYPE_GEOMETRY;
		}

		template <>
		Geometry *Node::as<Geometry>()
		{
			if (is<Geometry>())
			{
				return static_cast<Geometry *>(this);
			}
			return nullptr;
		}
	}
}