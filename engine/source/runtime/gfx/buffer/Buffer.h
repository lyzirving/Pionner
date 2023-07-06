#ifndef __GFX_BUFFER_H__
#define __GFX_BUFFER_H__

#include <memory>
#include <vector>

namespace pio
{
	namespace gfx
	{
		enum GfxBufType
		{
			GFX_BUF_VERTEX,
			GFX_BUF_INDICE,
			GFX_BUF_TEXTURE,
			GFX_BUF_CUBE_TEXTURE,
			GFX_BUF_DEPTH_FBO,
			GFX_BUF_CUBE_DEPTH_FBO,
			GFX_BUF_COLOR_FRAMEBUFFER,
			GFX_BUF_CNT
		};

		class Buffer
		{
		public:
			Buffer();
			Buffer(GfxBufType type);
			virtual ~Buffer();

			virtual void upload() = 0;

			virtual void bind();
			virtual void bindTarget(uint32_t target);
			virtual void unbind();

			virtual uint32_t size();
			virtual void     release();

			inline uint32_t   getId() const { return m_id; }
			inline GfxBufType getBufType() const { return m_bufferType; }
			inline bool       isUpload() const { return m_uploaded; }

			template<class T>
			bool is();

			template<class T>
			T *as();

		protected:
			inline bool isCreated() { return m_id > 0; }

		protected:
			uint32_t   m_id{0};
			GfxBufType m_bufferType{ GFX_BUF_CNT };
			bool       m_uploaded{false};
		};

		template<class T>
		bool Buffer::is() { return false; }

		template<class T>
		T *Buffer::as() { return nullptr; }
	}
}

#endif