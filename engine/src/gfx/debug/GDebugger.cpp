#include "GDebugger.h"

#include "gfx/renderer/Renderer.h"
#include "gfx/rhi/VertexArray.h"
#include "gfx/rhi/VertexBuffer.h"
#include "gfx/rhi/IndexBuffer.h"
#include "gfx/rhi/UniformBufferSet.h"

#include "asset/AssetsManager.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GDebugger"

namespace pio
{
	PIO_SINGLETON_IMPL(GDebugger)

	glm::vec4 GDebugger::Color = glm::vec4(1.f, 0.f, 0.f, 1.f);

	GDebugger::GDebugger()
	{
		m_dirty.reset();

		m_lines = AssetsManager::CreateRuntimeAssets<LineSegment>("DebugLine");
		m_lines->VertexBuffer = VertexBuffer::Create(LINE_MESH_LIMIT, BufferUsage::Dynamic);
		m_lines->VertexBuffer->setLayout(VertexBuffer::To<SimpleVertex>());
		m_lines->VertexArray = VertexArray::Create();
		m_lines->VertexArray->addVertexBuffer(m_lines->VertexBuffer);
		m_lines->IndexBuffer = IndexBuffer::Create(LINE_MESH_LIMIT, 0);
		m_lines->Capacity = LINE_MESH_LIMIT;
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

	void GDebugger::addLine(const Ray &r, float len)
	{
		if(!Renderer::GetConfig().Debugger.Raycast)
			return;

		addLine(r.Origin, r.Origin + r.Dir * len);
	}

	void GDebugger::addLine(const glm::vec3 &start, const glm::vec3 &end)
	{
		if(!Renderer::GetConfig().Debugger.Raycast)
			return;

		uint32_t byteUse = 2 * sizeof(SimpleVertex);
		if(m_lines->Capacity < m_lines->Size + byteUse)
		{
			LOGE("line mesh is out of memory");
			return;
		}
		uint32_t ind = m_lines->Vertex.size();
		m_lines->Vertex.emplace_back(start, glm::vec2(0.f));
		m_lines->Vertex.emplace_back(end, glm::vec2(0.f));
		m_lines->Indices.emplace_back(ind);
		m_lines->Indices.emplace_back(ind + 1);
		m_lines->Size += byteUse;

		m_dirty.set(GDebug_Line);
	}

	void GDebugger::drawLine(const Ref<UniformBufferSet> &ubs, const RenderState &state)
	{
		if(!Renderer::GetConfig().Debugger.Raycast || !any(GDebug_Line))
			return;

		flush(GDebug_Line);

		RenderState s = state;
		AssetHandle h = m_lines->getHandle();
		glm::vec4 &c = GDebugger::Color;

		Renderer::SubmitRC([h, c, ubs, s]() mutable
		{
			Renderer::RenderLineSegment(h, c, ubs, glm::mat4(1.f), s);
		});
	}

	bool GDebugger::any(GDebugType type)
	{
		switch (type)
		{
			case GDebug_Line:
			{
				return !m_lines->Vertex.empty() && !m_lines->Indices.empty();
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
			case GDebug_Line:
			{
				m_lines->clear();
				break;
			}
			default:
				break;
		}
	}

	void GDebugger::flush(GDebugType type)
	{
		if (!m_dirty.any())
			return;

		switch(type)
		{
			case GDebug_Line:
			{
				if(m_dirty.test(GDebug_Line))
				{
					m_dirty.reset(GDebug_Line);
					auto line = m_lines;
					Renderer::SubmitTask([line]() mutable
					{
						line->VertexBuffer->setData(line->Vertex.data(), sizeof(SimpleVertex) * line->Vertex.size());
						line->IndexBuffer->setData(line->Indices.data(), sizeof(uint32_t) * line->Indices.size(), line->Indices.size());
					});										
				}
				break;
			}
			default:
				break;
		}		
	}
}