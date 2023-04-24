#ifndef __GFX_BUFFER_H__
#define __GFX_BUFFER_H__

#include <memory>

#include "function/render/resource/ResourceDef.h"

namespace Pionner
{
	class Rhi;
	class RenderResourceMgr;

	class GfxBuffer
	{
	public:
		GfxBuffer(const std::shared_ptr<RenderResourceMgr> &mgr);
		virtual ~GfxBuffer();

		virtual void upload() = 0;
		virtual void release() = 0;

		virtual void insertData(uint32_t size, void *data);
		virtual void bindToTarget(uint32_t target);

		inline uint32_t getId() const { return m_id; }
		inline BufferType bufferType() const { return m_bufferType; }
		inline DataType dataType() const { return m_dataType; }
		inline bool isLoad() const { return m_loaded; }
		inline bool is(DataType type) { return m_dataType == type; }

	protected:
		friend class RenderResourceMgr;

		inline bool isCreated() { return m_id > 0; }

		std::weak_ptr<RenderResourceMgr> m_mgr;
		uint32_t   m_id;
		BufferType m_bufferType;
		DataType   m_dataType;
		bool       m_loaded;
	};
}

#endif