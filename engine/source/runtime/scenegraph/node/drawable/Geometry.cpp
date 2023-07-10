#include "Geometry.h"

#include "gfx/buffer/VertexBuffer.h"
#include "gfx/buffer/IndiceBuffer.h"

#include "render/shader/ShaderMgr.h"

namespace pio
{
	namespace sgf
	{
		Geometry::Geometry() : Drawable()
		{
			m_type = NODE_TYPE_GEOMETRY;
		}

		Geometry::Geometry(const std::string &name) : Drawable(name)
		{
			m_type = NODE_TYPE_GEOMETRY;
		}

		Geometry::~Geometry()
		{
		}

		void Geometry::addTexture(const std::shared_ptr<gfx::Texture> &texture)
		{
			m_textures.push_back(texture);
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

		void Geometry::setMaterial(const matl::Material &material)
		{
			m_material = material;
		}

		void Geometry::drawImplementation(RenderInfo &info)
		{
			if (!upload())
				return;

			renderMaterialDisplay();
		}

		bool Geometry::upload()
		{
			if (m_vertexs.empty() || m_indices.empty())
				return false;

			if (!m_vertexBuffer)
			{
				m_vertexBuffer = std::shared_ptr<gfx::Buffer>(new gfx::VertexBuffer);
				auto *p = m_vertexBuffer->as<gfx::VertexBuffer>();
				p->setVertexArray(m_vertexs);
				p->upload();
			}

			if (!m_indiceBuffer)
			{
				m_indiceBuffer = std::shared_ptr<gfx::Buffer>(new gfx::IndiceBuffer);
				auto *p = m_indiceBuffer->as<gfx::IndiceBuffer>();
				p->setIndiceArray(m_indices);
				p->upload();
			}

			return true;
		}

		void Geometry::renderMaterialDisplay()
		{

		}

		template <>
		bool Node::is<Geometry>()
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