#include "IndiceBuffer.h"

#include "scenegraph/SceneGrfDef.h"

#include "render/rhi/RhiHeader.h"

#include "core/log/LogSystem.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "IndiceBuffer"

namespace pio
{
	namespace gfx
	{
		IndiceBuffer::IndiceBuffer() : Buffer(GFX_BUF_INDICE)
		{
		}

		IndiceBuffer::~IndiceBuffer()
		{
			IndiceBuffer::release();
		}

		void IndiceBuffer::upload()
		{
			if (isUpload() && !m_dataChange.load())
				return;

			if (isCreated())
				release();

			if (m_indiceArray.empty())
				return;

			glGenBuffers(1, &m_id);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
						 m_indiceArray.size() * sizeof(uint32_t), 
						 &m_indiceArray[0], 
						 GL_STATIC_DRAW);

			m_uploaded = GLHelper::checkGLErr("fail to build indice buffer");
			m_dataChange.store(false);
		}

		void IndiceBuffer::bind()
		{
			if (!isUpload() || m_dataChange.load())
			{
				upload();
			}

			if (isUpload())
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
			}
		}

		void IndiceBuffer::unbind()
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		uint32_t IndiceBuffer::size()
		{
			if (m_indiceArray.empty())
			{
				return 0;
			}
			else
			{
				return m_indiceArray.size() * sizeof(uint32_t);
			}
		}

		void IndiceBuffer::release()
		{
			if (isCreated())
			{
				glDeleteBuffers(1, &m_id);
				m_id = 0;
			}
		}

		void IndiceBuffer::setIndiceArray(const std::vector<uint32_t> &indiceArray)
		{
			m_indiceArray.clear();
			if (!indiceArray.empty())
			{
				m_indiceArray.assign(indiceArray.begin(), indiceArray.end());
				m_dataChange.store(true);
			}
		}

		template<>
		bool Buffer::is<IndiceBuffer>()
		{
			return m_bufferType == GFX_BUF_INDICE;
		}

		template<>
		IndiceBuffer *Buffer::as<IndiceBuffer>()
		{
			if (is<IndiceBuffer>())
			{
				return static_cast<IndiceBuffer *>(this);
			}
			return nullptr;
		}
	}
}