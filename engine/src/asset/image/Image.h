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

		void SetData(uint8_t** data, int32_t w, int32_t h, int32_t comp);

		uint8_t* Data() { return m_Data; }
		const uint8_t* Data() const { return m_Data; }

		Ref<Texture>& GetTexture() { return m_Texture; }
		const Ref<Texture>& GetTexture() const { return m_Texture; }

		int32_t GetChannelNum() const { return m_Comp; }

	public:
		static ImageFormat ParseFormat(const std::string& fullPath);
		static TextureFormat ParseTextureFmt(int32_t comp);

	private:
		int32_t m_Width{ 0 }, m_Height{ 0 }, m_Comp{ 0 };
		ImageFormat m_Format{ ImageFormat_PNG };
		uint8_t* m_Data{ nullptr };
		TextureParams m_TextureParams;
		Ref<Texture> m_Texture;

		friend class ImageImporter;
	};
}