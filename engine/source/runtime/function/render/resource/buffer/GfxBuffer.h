#ifndef __GFX_BUFFER_H__
#define __GFX_BUFFER_H__

#include <memory>
#include <vector>
#include "function/render/resource/ResourceDef.h"

namespace Pionner
{
	class Rhi;
	template <class T>
	class BufferArray;
	class RenderResourceMgr;

	class GfxBuffer
	{
	public:
		GfxBuffer(const std::shared_ptr<RenderResourceMgr> &mgr);
		GfxBuffer(bool holder, const std::shared_ptr<RenderResourceMgr> &mgr);
		virtual ~GfxBuffer();

		virtual void upload() = 0;

		virtual void bind();
		virtual void bindTarget(uint32_t target);
		virtual void unbind();

		virtual void deleteResource();
		virtual void loadRawData();
		virtual uint32_t size();

		inline uint32_t getId() const { return m_id; }
		inline uint32_t getSlot() const { return m_slot; }
		inline BufferType getBufferType() const { return m_bufferType; }
		inline bool isUpload() const { return m_uploaded; }
		inline void setHolderId(uint32_t id) { if (m_isHolder) m_id = id; }

		void notifyRelease();

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

		template <class T>
		friend class BufferArray;

		inline bool isAbandonded() { return m_abandoned; }
		inline bool isCreated() { return m_id > 0; }

		std::weak_ptr<RenderResourceMgr> m_mgr;
		uint32_t   m_id;
		uint32_t   m_slot;
		BufferType m_bufferType;
		bool       m_uploaded;
		bool       m_abandoned;
		bool       m_isHolder;
	};
}

#endif