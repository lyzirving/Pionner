#include "View.h"

#include "gfx/renderer/Renderer.h"
#include "gfx/rhi/Texture.h"
#include "gfx/struct/MeshFactory.h"
#include "gfx/struct/Geometry2D.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "View"

namespace pio
{
	void View::upload()
	{
		if (!needUpdate())
			return;

		invalidate(false);
		if (!m_quad)
		{
			m_quad = MeshFactory::CreateScreenQuad(m_rect.Left, m_rect.Top, m_rect.Right, m_rect.Bottom, m_viewport.w(), m_viewport.h());
		}
		else
		{
			QuadMesh *mesh = m_quad->as<QuadMesh>();
			mesh->Vertex.clear();
			mesh->Vertex.reserve(4);
			mesh->Vertex.emplace_back(Math::ScreenPtToVertex(m_rect.Left, m_rect.Top, m_viewport.w(), m_viewport.h()), glm::vec2(0.f, 1.f));
			mesh->Vertex.emplace_back(Math::ScreenPtToVertex(m_rect.Left, m_rect.Bottom, m_viewport.w(), m_viewport.h()), glm::vec2(0.f, 0.f));
			mesh->Vertex.emplace_back(Math::ScreenPtToVertex(m_rect.Right, m_rect.Top, m_viewport.w(), m_viewport.h()), glm::vec2(1.f, 1.f));
			mesh->Vertex.emplace_back(Math::ScreenPtToVertex(m_rect.Right, m_rect.Bottom, m_viewport.w(), m_viewport.h()), glm::vec2(1.f, 0.f));
			Renderer::SubmitTask([mesh]() mutable
			{
				mesh->VertexBuffer->setData(mesh->Vertex.data(), mesh->Vertex.size() * sizeof(QuadVertex));
			});
		}
	}

	AssetHandle View::getMesh() const
	{
		return m_quad.get() != nullptr ? m_quad->getHandle() : NullAsset;
	}
}