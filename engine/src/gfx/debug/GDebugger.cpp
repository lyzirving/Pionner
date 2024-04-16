#include "GDebugger.h"

#include "gfx/renderer/Renderer.h"
#include "gfx/rhi/VertexArray.h"
#include "gfx/rhi/VertexBuffer.h"
#include "gfx/rhi/IndexBuffer.h"

#include "asset/AssetsManager.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GDebugger"

namespace pio
{
	PIO_SINGLETON_IMPL(GDebugger)

	GDebugger::GDebugger()
	{
		m_dirty.reset();

		m_lineMesh = AssetsManager::CreateRuntimeAssets<LineMesh>("DebugLine");
		m_lineMesh->VertexBuffer = VertexBuffer::Create(LINE_MESH_LIMIT, BufferUsage::Dynamic);
		m_lineMesh->VertexBuffer->setLayout(VertexBuffer::To<LineVertex>());
		m_lineMesh->VertexArray = VertexArray::Create();
		m_lineMesh->VertexArray->addVertexBuffer(m_lineMesh->VertexBuffer);
		m_lineMesh->IndexBuffer = IndexBuffer::Create(LINE_MESH_LIMIT, 0);
		m_lineMesh->Size = LINE_MESH_LIMIT;
	}

	GDebugger::~GDebugger() = default;

	void GDebugger::Init()
	{
		GDebugger::Get();
	}

	void GDebugger::Shutdown()
	{
		std::lock_guard<std::mutex> lk{ s_mutex };
		delete s_instance;
		s_instance = nullptr;
	}

	void GDebugger::drawLine(const Ray &r, float len, const glm::vec4 &color)
	{
		if (!Renderer::GetConfig().Debugger.Raycast)
			return;
		drawLine(r.Origin, r.Origin + r.Dir * len, color);
	}

	void GDebugger::drawLine(const glm::vec3 &start, const glm::vec3 &end, const glm::vec4 &color)
	{
		uint32_t byteUse = 2 * sizeof(LineVertex);
		if (m_lineMesh->Size < m_lineMesh->Capacity + byteUse)
		{
			LOGE("line mesh is out of memory");
			return;
		}
		uint32_t ind = m_lineMesh->Vertex.size();
		m_lineMesh->Vertex.emplace_back(start, color);
		m_lineMesh->Vertex.emplace_back(end, color);
		m_lineMesh->Indices.emplace_back(ind);
		m_lineMesh->Indices.emplace_back(ind + 1);
		m_lineMesh->Capacity += byteUse;

		m_dirty.set(PIO_UINT8(GDebugType::Line));
	}

	bool GDebugger::any(GDebugType type)
	{
		switch (type)
		{
			case GDebugType::Line:
			{
				return !m_lineMesh->Vertex.empty() && !m_lineMesh->Indices.empty();
			}
			default:
				break;
		}
		return false;
	}

	void GDebugger::clear(GDebugType type)
	{
		switch (type)
		{
			case GDebugType::Line:
			{
				m_lineMesh->clear();
				break;
			}
			default:
				break;
		}
	}

	void GDebugger::flush()
	{
		if (!m_dirty.any())
			return;

		if (m_dirty.test(PIO_UINT8(GDebugType::Line)))
		{
			m_lineMesh->VertexBuffer->setData(m_lineMesh->Vertex.data(), sizeof(LineVertex) * m_lineMesh->Vertex.size());
			m_lineMesh->IndexBuffer->setData(m_lineMesh->Indices.data(), sizeof(uint32_t) * m_lineMesh->Indices.size(), m_lineMesh->Indices.size());
			m_dirty.reset(PIO_UINT8(GDebugType::Line));
		}
	}
}