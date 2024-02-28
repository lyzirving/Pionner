#include "View.h"

#include "gfx/rhi/Texture.h"
#include "gfx/rhi/VertexArray.h"
#include "gfx/rhi/VertexBuffer.h"
#include "gfx/rhi/IndexBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "View"

namespace pio
{
	void IconView::upload()
	{
		if (!needUpdate())
			return;

		invalidate(false);
		glm::vec2 v0 = UiDef::ScreenToVertex(m_rect.Left, m_rect.Top, m_viewport.Width, m_viewport.Height);
		glm::vec2 v1 = UiDef::ScreenToVertex(m_rect.Left, m_rect.Bottom, m_viewport.Width, m_viewport.Height);
		glm::vec2 v2 = UiDef::ScreenToVertex(m_rect.Right, m_rect.Top, m_viewport.Width, m_viewport.Height);
		glm::vec2 v3 = UiDef::ScreenToVertex(m_rect.Right, m_rect.Bottom, m_viewport.Width, m_viewport.Height);
		
		std::vector<ViewVertex> vertex;
		vertex.reserve(4);
		vertex.emplace_back(glm::vec3(v0.x, v0.y, 0.f), glm::vec2(0.f, 1.f));
		vertex.emplace_back(glm::vec3(v1.x, v1.y, 0.f), glm::vec2(0.f, 0.f));
		vertex.emplace_back(glm::vec3(v2.x, v2.y, 0.f), glm::vec2(1.f, 1.f));
		vertex.emplace_back(glm::vec3(v3.x, v3.y, 0.f), glm::vec2(1.f, 0.f));
		std::vector<uint32_t> indice{0, 1, 2, 2, 1, 3};		

		if (!m_vertexArray)
		{
			m_vertexArray = VertexArray::Create();			
			m_vertexBuffer = VertexBuffer::Create(vertex.data(), vertex.size() * sizeof(ViewVertex));
			m_indexBuffer = IndexBuffer::Create(indice.data(), indice.size() * sizeof(uint32_t), indice.size());

			m_vertexBuffer->setLayout(VertexBuffer::To<ViewVertex>());
			m_vertexArray->addVertexBuffer(m_vertexBuffer);
		}
		else
		{
			//Update vertex buffer
			m_vertexBuffer->setData(vertex.data(), vertex.size() * sizeof(ViewVertex));
		}
	}
}