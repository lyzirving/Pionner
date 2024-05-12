#include "ImageImporter.h"
#include "ImageUtils.h"

#include "asset/AssetsManager.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ImageImporter"

namespace pio
{
	ImageImporter::ImageImporter(const std::string &name, AssetFmt fmt) 
		: m_name(name), m_fmt(fmt), 
		  m_assetsDir(AssetsManager::AssetsRootDirectory(fmt) + "/" + name + "/")
	{
	}

	bool ImageImporter::importToImage(Image &image/*out*/)
	{
		std::string absPath;
		if (!AssetsManager::FindAssetAbsPath(m_assetsDir, m_fmt, absPath))
		{
			LOGE("fail to find assets[%s][%s]", m_assetsDir.c_str(), AssetsManager::GetFmtPostfix(m_fmt));
			return false;
		}
		image.AFmt = m_fmt;
		image.Name = m_name;
		image.Path = absPath;	
		image.ParentDir = absPath.substr(0, absPath.find_last_of("/"));
		if (m_fmt == AssetFmt::HDR)
		{
			stbi_set_flip_vertically_on_load(true);
			// Note that we load image with float value
			image.m_data = stbi_loadf(absPath.c_str(), &image.Width, &image.Height, &image.Channel, 0);
			if (!image.loaded())
				LOGE("fail to load hdr data from path[%s]", absPath.c_str());
			stbi_set_flip_vertically_on_load(false);
			return image.loaded();
		}
		else
		{
			LOGE("invalid assets fmt[%u]", m_fmt);
			return false;
		}
		return false;
	}
}