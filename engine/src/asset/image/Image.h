#pragma once

#include "asset/Asset.h"

namespace pio
{
	class Texture;

	class Image : public Asset
	{
		RTTR_ENABLE(Asset)
	public:
		Image(const ImportParams& params);
		virtual ~Image() = default;
		virtual bool IsInit() const override { return m_Texture.use_count() != 0; }
		virtual void OnInit() override;

		void SetData(void** data, int32_t w, int32_t h, int32_t comp);

		void* Data() { return m_Data; }
		const void* Data() const { return m_Data; }

		Ref<Texture>& GetTexture() { return m_Texture; }
		const Ref<Texture>& GetTexture() const { return m_Texture; }

		int32_t GetChannelNum() const { return m_Comp; }

		int32_t GetWidth() const { return m_Width; }
		int32_t GetHeight() const { return m_Height; }

	public:
		static ImageFormat ParseFormat(const std::string& fullPath);
		static TextureFormat ParseTextureFmt(int32_t comp);

	protected:
		int32_t m_Width{ 0 }, m_Height{ 0 }, m_Comp{ 0 };
		ImageFormat m_Format{ ImageFormat_PNG };
		void* m_Data{ nullptr };
		TextureParams m_TextureParams;
		Ref<Texture> m_Texture;

	private:
		friend class ImageImporter;
	};
}