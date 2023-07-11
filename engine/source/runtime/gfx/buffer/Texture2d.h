#ifndef __PIONNER_GFX_BUFFER_TEXTURE_2D_H__
#define __PIONNER_GFX_BUFFER_TEXTURE_2D_H__

#include "Texture.h"

namespace pio
{
	namespace gfx
	{
		enum SurfaceType : uint8_t
		{
			SURFACE_AMBIENT,
			SURFACE_DIFFUSE,
			SURFACE_SPECULAR,
			SURFACE_NORMAL,
			SURFACE_HEIGHT,
			SURFACE_CNT
		};

		class Texture2d : public Texture
		{
		public:
			Texture2d();
			Texture2d(SurfaceType type);
			virtual ~Texture2d();

			virtual void upload() override;

			virtual void bind() override;
			virtual void bindTarget(uint32_t target) override;
			virtual void unbind() override;
			virtual uint32_t size() override;
			virtual void release() override;

			inline SurfaceType getSurfaceType() { return m_surfaceType; }

		private:
			SurfaceType m_surfaceType{ SURFACE_CNT };
		};

		template<>
		bool Buffer::is<Texture2d>();

		template<>
		Texture2d *Buffer::as<Texture2d>();
	}
}

#endif