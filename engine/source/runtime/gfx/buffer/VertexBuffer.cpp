#include "VertexBuffer.h"

#include "scenegraph/SceneGrfDef.h"

#include "render/rhi/RhiHeader.h"

#include "core/log/LogSystem.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "VertexBuffer"

namespace pio
{
	namespace gfx
	{
		VertexBuffer::VertexBuffer() : Buffer(GFX_BUF_VERTEX)
		{
		}

		VertexBuffer::~VertexBuffer()
		{
			VertexBuffer::release();
		}

		void VertexBuffer::upload()
		{
			if (isUpload() && !m_dataChange.load())
				return;

			if (isCreated())
				release();

			if (m_vertexArray.empty())
				return;

			// TODO: consider about the case when the vertex array changes

			glGenVertexArrays(1, &m_vao);
			glGenBuffers(1, &m_id);

			// step 1: upload data 
			glBindBuffer(GL_ARRAY_BUFFER, m_id);
			glBufferData(GL_ARRAY_BUFFER, 
						 m_vertexArray.size() * sizeof(sgf::Vertex), 
						 &m_vertexArray[0], 
						 GL_STATIC_DRAW);

			// step 2: bind data to vao 
			glBindVertexArray(m_vao);

			// bind buffer to vao
			glBindBuffer(GL_ARRAY_BUFFER, m_id);
			// set vertex position
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(sgf::Vertex), (void *)nullptr);
			// set vertex normal
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(sgf::Vertex), (void *)offsetof(sgf::Vertex, normal));
			// set texture coordinates
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(sgf::Vertex), (void *)offsetof(sgf::Vertex, texCoord));
			// set tangents
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(sgf::Vertex), (void *)offsetof(sgf::Vertex, tangents));

			// step 3: unbind vao
			glBindVertexArray(0);

			m_uploaded = gfx::GLHelper::checkGLErr("fail to build vertex buffer");
			m_dataChange.store(false);
		}

		void VertexBuffer::bind()
		{
			if (!isUpload() || m_dataChange.load())
			{
				upload();
			}

			if (isUpload())
			{
				glBindVertexArray(m_vao);
			}
		}

		void VertexBuffer::unbind()
		{
			glBindVertexArray(0);
		}

		uint32_t VertexBuffer::size()
		{
			return m_vertexArray.size();
		}

		void VertexBuffer::release()
		{
			if (isCreated())
			{
				glDeleteBuffers(1, &m_id);
				glDeleteVertexArrays(1, &m_vao);
				m_id = 0;
				m_vao = 0;
			}
		}

		void VertexBuffer::setVertexArray(const std::vector<sgf::Vertex> &vertexArray)
		{
			m_vertexArray.clear();
			if (!vertexArray.empty())
			{
				m_vertexArray.assign(vertexArray.begin(), vertexArray.end());
				m_dataChange.store(true);
			}
		}

		template<>
		bool Buffer::is<VertexBuffer>()
		{
			return m_bufferType == GFX_BUF_VERTEX;
		}

		template<>
		VertexBuffer *Buffer::as<VertexBuffer>()
		{
			if (is<VertexBuffer>())
			{
				return static_cast<VertexBuffer *>(this);
			}
			return nullptr;
		}
	}
}