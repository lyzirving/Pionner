#ifndef __PIONNER_PLATFORM_OPENGL_GL_TEXTURE_H__
#define __PIONNER_PLATFORM_OPENGL_GL_TEXTURE_H__

#include "gfx/rhi/Texture.h"

namespace pio
{
	class GLTexture
	{
	public:
		GLTexture() {}
		GLTexture(const TextureSpecification &spec);
		virtual ~GLTexture() = default;

	protected:
		uint32_t m_textureId{ 0 };
		uint32_t m_size{ 0 };

		TextureSpecification m_spec{};
		TextureInternalFmt m_internalFmt{ TextureInternalFmt::None };
		TextureFmt m_textureFmt{ TextureFmt::None };
		PixelType m_pixelType{ PixelType::None };
	};

	class GLTexture2D : public Texture2D, public GLTexture
	{
	public:
		GLTexture2D(const std::string &path, const TextureSpecification &spec);
		GLTexture2D(const TextureSpecification &spec, const Ref<Buffer> &buffer);
		GLTexture2D(const TextureSpecification &spec);
		virtual ~GLTexture2D();

		virtual uint32_t getId() const override { return m_textureId; }
		virtual ImageInternalFormat getFormat() const override { return m_spec.Format; }
		virtual uint32_t getWidth() const override { return m_spec.Width; }
		virtual uint32_t getHeight() const override { return m_spec.Height; }
		virtual uint32_t getSize() const override { return m_size; }
		virtual const std::string &getName() const override { return m_spec.Name; }
		virtual uint32_t getMipLevelCount() const override { return m_spec.MaxMipLevel; }
		virtual bool SRGB() const override { return m_spec.SRGB; };

		virtual bool init() override;
		virtual void destroy() override;
		virtual bool isInit() const override { return m_textureId != 0; }

		virtual void active(uint32_t sampler = 0) override;
		virtual void bind() override;
		virtual void unbind() override;

		virtual Ref<Buffer> getBuffer() override { return m_imageData; }

	private:
		Ref<Buffer> m_imageData{ nullptr };
	};

	class GLCubeTexture : public CubeTexture, public GLTexture
	{
	public:
		GLCubeTexture(const TextureSpecification &spec);
		virtual ~GLCubeTexture();

		virtual uint32_t getId() const override { return m_textureId; }
		virtual ImageInternalFormat getFormat() const override { return m_spec.Format; }
		virtual uint32_t getWidth() const override { return m_spec.Width; }
		virtual uint32_t getHeight() const override { return m_spec.Height; }
		virtual uint32_t getSize() const override { return m_size; }
		virtual const std::string &getName() const override { return m_spec.Name; }
		virtual uint32_t getMipLevelCount() const override { return m_spec.MaxMipLevel; }
		virtual Ref<Buffer> getBuffer() override { return Ref<Buffer>(); };
		virtual bool SRGB() const override { return m_spec.SRGB; };

		virtual bool init() override;
		virtual void destroy() override;
		virtual bool isInit() const override { return m_textureId != 0; }

		virtual void active(uint32_t sampler = 0) override;
		virtual void bind() override;
		virtual void unbind() override;
	};

	class GLCubeArrayTexture : public CubeArrayTexture, public GLTexture
	{
	public:
		GLCubeArrayTexture(const TextureSpecification &spec);
		virtual ~GLCubeArrayTexture();

		virtual bool init() override;
		virtual void destroy() override;

		virtual void active(uint32_t sampler = 0) override;
		virtual void bind() override;
		virtual void unbind() override;

		virtual ImageInternalFormat getFormat() const override { return m_spec.Format; }
		virtual uint32_t getWidth() const override { return m_spec.Width; }
		virtual uint32_t getHeight() const override { return m_spec.Height; }
		virtual uint32_t getSize() const override { return m_size; }

		virtual bool isInit() const override { return m_textureId != 0; }
		virtual const std::string &getName() const override { return m_spec.Name; }
		virtual uint32_t getId() const override { return m_textureId; }
		virtual uint32_t getMipLevelCount() const override { return 0; }
		virtual bool SRGB() const override { return m_spec.SRGB; };
	};

	class GLBufferTexture : public BufferTexture, public GLTexture
	{
	public:
		GLBufferTexture(void *data, uint32_t byteSize, ImageInternalFormat format);
		virtual ~GLBufferTexture();

		virtual uint32_t getId() const override { return m_textureId; }
		virtual ImageInternalFormat getFormat() const override { return m_spec.Format; }
		virtual uint32_t getWidth() const override { return 0; }
		virtual uint32_t getHeight() const override { return 0; }
		virtual uint32_t getSize() const override { return m_size; }
		virtual const std::string &getName() const override { return "BufferTexture"; }
		virtual uint32_t getMipLevelCount() const override { return 0; }
		virtual bool SRGB() const override { return m_spec.SRGB; };

		virtual bool init() override;
		virtual void destroy() override;
		virtual bool isInit() const override { return m_textureId != 0; }

		virtual void active(uint32_t sampler = 0) override;
		virtual void bind() override;
		virtual void unbind() override;

	private:
		uint32_t m_bufferId{ 0 };
		void *m_data{ nullptr };
	};
}

#endif