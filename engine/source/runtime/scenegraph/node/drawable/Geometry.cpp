#include "Geometry.h"

#include "render/rhi/RhiHeader.h"

#include "gfx/buffer/VertexBuffer.h"
#include "gfx/buffer/IndiceBuffer.h"

#include "gfx/shader/ShaderManager.h"
#include "gfx/buffer/Texture2d.h"

#include "core/log/LogSystem.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Geometry"

namespace pio
{
	using namespace gfx;

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
			if (!m_vertexBuffer || !m_indiceBuffer)
				return;

			auto shader = ShaderManager::self()->get(SHADER_TYPE_MESH);
			if (!shader)
			{
				LOG_ERR("mesh shader is invalid");
				return;
			}

			shader->use(true);

			// Find diffuse texture
			int32_t textureUnit{ 0 };
			for (auto &texture : m_textures)
			{
				Texture2d *p{ nullptr };
				if (!texture || !(p = texture->as<Texture2d>()))
					continue;

				if (p->getSurfaceType() == SURFACE_DIFFUSE)
				{
					// Only upload diffuse in material display mode.
					p->upload();
					p->bindTarget(textureUnit);
					shader->setInt("u_material.diffuseTexture", textureUnit++);
					shader->setInt("u_material.hasDiffTex", 1);
					break;
				} 
			}

			// Diffuse texture does not exist
			if (textureUnit == 0)
			{
				shader->setInt("u_material.hasDiffTex", 0);
			}

			shader->setVec3("u_material.ka", m_material.getAmbient());
			shader->setVec3("u_material.kd", m_material.getDifffuse());
			shader->setVec3("u_material.ks", m_material.getSpecular());

			m_vertexBuffer->upload();
			m_indiceBuffer->upload();

			m_vertexBuffer->bind();
			m_indiceBuffer->bind();

			glDrawElements(GL_TRIANGLES, m_indiceBuffer->size(), GL_UNSIGNED_INT, nullptr);
			bool success = GLHelper::checkGLErr("err happens when drawing part");
			if (!success)
			{
				LOG_ERR("fail to draw geometry[%s]draw failed, material name[%s]", m_name.c_str(), m_material.getName().c_str());
			}

			m_vertexBuffer->unbind();
			m_indiceBuffer->unbind();

			glBindTexture(GL_TEXTURE_2D, 0);

			shader->use(false);
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