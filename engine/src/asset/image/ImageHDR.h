#pragma once

#include "Image.h"

namespace pio
{
	class CubeMapBuildPass;

	class ImageHDR : public Image
	{
		RTTR_ENABLE(Image)
	public:
		ImageHDR(const ImportParams& params);
		virtual ~ImageHDR() = default;		
		virtual void OnInit() override;

	private:
		void BuildFromEquirectangular();

	private:
		// Texture for Equirectangular image
		Ref<Texture> m_SourceTexture;
		Ref<CubeMapBuildPass> m_ToolPass;

	private:
		friend class ImageImporter;
		friend class CubeMapBuildPass;
	};
}