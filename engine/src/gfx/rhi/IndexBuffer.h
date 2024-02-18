#ifndef __PIONNER_GFX_RHI_INDEX_BUFFER_H__
#define __PIONNER_GFX_RHI_INDEX_BUFFER_H__

#include "RhiDef.h"

namespace pio
{
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual bool init() = 0;
		virtual void destroy() = 0;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void setData(const void *buffer, uint32_t size, uint32_t indexCnt, uint32_t offset = 0) = 0;

		virtual bool isInit() const = 0;
		virtual uint32_t getSize() const = 0;
		virtual uint32_t getCount() const = 0;

	public:
		static Ref<IndexBuffer> Create(uint32_t size, uint32_t num);
		static Ref<IndexBuffer> Create(const void *data, uint32_t size, uint32_t num);
	};
}

#endif