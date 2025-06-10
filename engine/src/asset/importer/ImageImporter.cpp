#include "ImageImporter.h"
#include "asset/image/Image.h"

#include "base/utils/ImageUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ImageImporter"

namespace pio
{
	ImageImporter::ImageImporter(const ImportParams& params) : Importer(params)
	{
	}

	Ref<Asset> ImageImporter::Load()
	{
		if (m_ImportParams.Setting.Image.bUseData)
		{
			return LoadFromData();
		}
		else
		{
			return LoadFromPath();
		}
	}

	Ref<Asset> ImageImporter::LoadFromPath()
	{
		ImageFormat imageFormat = Image::ParseFormat(m_FullPath);
		if (imageFormat == ImageFormat_None)
			return std::shared_ptr<Image>();

		uint8_t* data{ nullptr };
		int32_t width{ 0 }, height{ 0 }, comp{ 0 };
		if (!ImageUtil::LoadImage(m_FullPath.c_str(), &data, width, height, comp))
		{
			LOGE("fail to load image from[%s]", m_FullPath.c_str());
			return std::shared_ptr<Asset>();
		}
		LOGD("succed to load image[%s], [%u, %u, %u]", m_FullPath.c_str(), width, height, comp);
		//[BugFix] some model's baseColor texture is targeted as 1 component
		if(m_ImportParams.Setting.Image.Comp > comp && comp == 1)
		{
			LOGD("extend image's channel[%s] from[%u] to[%u]", m_FullPath.c_str(), comp, m_ImportParams.Setting.Image.Comp);
			comp = m_ImportParams.Setting.Image.Comp;
			uint8_t* extend = (uint8_t *)std::malloc(width * height * comp);
			for(size_t i = 0; i < height; i++)
			{
				for(size_t j = 0; j < width; j++)
				{					
					auto val = data[i * width + j];
					auto idx = i * width * comp + j * comp;
					for(size_t k = 0; k < comp; k++)
					{						
						extend[idx + k] = (k == 3) ? 255 : val;
					}
				}
			}
			std::free(data);
			data = extend;
		}
		auto image = CreateRef<Image>(m_ImportParams);
		image->m_Width = width;
		image->m_Height = height;
		image->m_Comp = comp;
		image->m_Format = imageFormat;
		image->m_Data = data;
		image->m_TextureParams = m_ImportParams.Setting.Image.Param;
		return image;
	}

	Ref<Asset> ImageImporter::LoadFromData()
	{
		auto image = CreateRef<Image>(m_ImportParams);
		image->m_Width = m_ImportParams.Setting.Image.Width;
		image->m_Height = m_ImportParams.Setting.Image.Height;
		image->m_Comp = m_ImportParams.Setting.Image.Comp;
		image->m_Format = GetFormatByChannal(image->m_Comp);
		image->m_TextureParams = m_ImportParams.Setting.Image.Param;
		image->m_Data = m_ImportParams.Setting.Image.Data;
		m_ImportParams.Setting.Image.Data = nullptr;
		return image;
	}
}