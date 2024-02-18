#ifndef __PIONNER_ASSET_IMAGE_IMPORTER_H__
#define __PIONNER_ASSET_IMAGE_IMPORTER_H__

#include "gfx/struct/Image.h"

namespace pio
{
	class ImageImporter
	{
	public:
		ImageImporter(const std::string &name, AssetFmt fmt);

		bool importToImage(Image &image/*out*/);

	private:
		const std::string m_name;
		const std::string m_assetsDir;
		const AssetFmt m_fmt;
	};
}

#endif