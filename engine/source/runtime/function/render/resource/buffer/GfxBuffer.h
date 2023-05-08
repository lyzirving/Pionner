#ifndef __GFX_BUFFER_H__
#define __GFX_BUFFER_H__

#include <memory>
#include <vector>
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

		virtual void bind();
		virtual void bindToTarget(uint32_t target);
		virtual uint32_t size();
		virtual void load();

		inline uint32_t getId() const { return m_id; }
		inline uint32_t getSlot() const { return m_slot; }
		inline BufferType getBufferType() const { return m_bufferType; }
		inline DataType getDataType() const { return m_dataType; }
		inline bool isUpload() const { return m_uploaded; }

		template<class T>
		bool is() const;

		template<class T>
		T *getPtr();

		template<class T>
		void insertData(T &data);

		template<class T>
		void insertData(std::vector<T> &data);

	protected:
		friend class RenderResourceMgr;

		inline bool isCreated() { return m_id > 0; }

		std::weak_ptr<RenderResourceMgr> m_mgr;
		uint32_t   m_id;
		uint32_t   m_slot;
		BufferType m_bufferType;
		DataType   m_dataType;
		bool       m_uploaded;
	};
}

#endif