#ifndef __GFX_BUFFER_H__
#define __GFX_BUFFER_H__

#include <memory>
#include <vector>
#include "render/resource/ResourceDef.h"

namespace pio
{
	namespace gfx
	{
		class Buffer
		{
		public:
			Buffer();
			virtual ~Buffer();

			virtual void upload() = 0;

			virtual void bind();
			virtual void bindTarget(uint32_t target);
			virtual void unbind();

			virtual void     load();
			virtual uint32_t size();
			virtual void     release();

			inline uint32_t   getId() const { return m_id; }
			inline BufferType getBufferType() const { return m_bufferType; }
			inline bool       isUpload() const { return m_uploaded; }

			template<class T>
			bool is() const;

			template<class T>
			T *as();

		protected:
			inline bool isCreated() { return m_id > 0; }

		protected:
			uint32_t   m_id;
			BufferType m_bufferType;
			bool       m_uploaded;
		};

		template<class T>
		bool Buffer::is() const { return false; }

		template<class T>
		T *Buffer::as() { return nullptr; }
	}
}

#endif