#pragma once

#include "Importer.h"

namespace pio
{
	class ImageImporter : public Importer
	{
		RTTR_ENABLE(Importer)
	public:
		ImageImporter(const ImportParams& params);
		virtual ~ImageImporter() = default;
		virtual Ref<Asset> Load() override;

	private:
		Ref<Asset> LoadFromPath();
		Ref<Asset> LoadFromData();

		Ref<Asset> LoadImage2D(ImageFormat format);
		Ref<Asset> LoadImageHDR(ImageFormat format);

	private:
		static bool IsImage2D(ImageFormat format);
	};
}