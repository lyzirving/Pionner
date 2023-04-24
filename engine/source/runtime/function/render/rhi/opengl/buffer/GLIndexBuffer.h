#ifndef __INDEX_BUFFER_H__
#define __INDEX_BUFFER_H__

#include "function/render/resource/buffer/GfxBuffer.h"

namespace Pionner
{
	class GLIndexBuffer : public GfxBuffer
	{
		friend class GfxBuffer;
	public:
		GLIndexBuffer(const std::shared_ptr<RenderResourceMgr> &mgr);
		virtual ~GLIndexBuffer();

		virtual void upload() override;
		virtual void release() override;

		virtual void bind() override;

	private:
		std::vector<uint32_t> m_indices;
	};

	template<>
	bool GfxBuffer::is<GLIndexBuffer>() const
	{
		return getDataType() == DATA_INDICE;
	}

	template<>
	void GfxBuffer::insertData<uint32_t>(std::vector<uint32_t> &data)
	{
		if (is<GLIndexBuffer>())
		{
			GLIndexBuffer *ptr = static_cast<GLIndexBuffer *>(this);
			ptr->m_indices.assign(data.begin(), data.end());
		}
	}
}

#endif