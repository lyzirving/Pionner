#ifndef __PIONNER_PLATFORM_GL_INDEX_BUFFER_H__
#define __PIONNER_PLATFORM_GL_INDEX_BUFFER_H__

#include "gfx/rhi/IndexBuffer.h"

namespace pio
{
	class GLIndexBuffer : public IndexBuffer
	{
	public:
		GLIndexBuffer(uint32_t size, uint32_t num);
		GLIndexBuffer(const void *data, uint32_t size, uint32_t num);
		virtual ~GLIndexBuffer();

		virtual bool init() override;
		virtual void destroy() override;

		virtual void bind() override;
		virtual void unbind() override;

		virtual void setData(const void *data, uint32_t size, uint32_t indexCnt, uint32_t offset = 0) override;

		virtual bool isInit() const { return m_vbo != 0; }
		virtual uint32_t getSize() const override { return m_size; }
		virtual uint32_t getCount() const override { return m_num; };

	private:
		uint32_t m_vbo{ 0 };
		uint32_t m_size{ 0 };
		uint32_t m_num{ 0 };
		void *m_data{ nullptr };
	};
}

#endif