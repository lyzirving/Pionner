#ifndef __INDEX_BUFFER_H__
#define __INDEX_BUFFER_H__

#include "render/resource/buffer/GfxBuffer.h"

namespace pio
{
	class GLIndexBuffer : public GfxBuffer
	{
		friend class GfxBuffer;
	public:
		GLIndexBuffer(const std::shared_ptr<RenderResourceMgr> &mgr);
		virtual ~GLIndexBuffer();

		virtual void upload() override;

		virtual void bind() override;
		virtual void unbind() override;
		virtual void deleteResource() override;
		virtual uint32_t size() override { return m_indices.size(); }

	private:
		std::vector<uint32_t> m_indices;
	};

	template<>
	bool GfxBuffer::is<GLIndexBuffer>() const;

	template<>
	void GfxBuffer::insertData<uint32_t>(std::vector<uint32_t> &data);
}

#endif