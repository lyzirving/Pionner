#ifndef __PIONNER_SCENE_SKYBOX_H__
#define __PIONNER_SCENE_SKYBOX_H__

#include "gfx/struct/Image.h"
#include "gfx/renderer/RenderData.h"

namespace pio
{
	class FrameBuffer;
	class Texture2D;
	class RenderPass;
	class CubeTexture;

	class Skybox : public Asset
	{
		OVERRIDE_ASSET_TYPE(AssetType::Skybox)
	public:
		Skybox(const std::string &name, AssetFmt fmt);
		~Skybox();

	public:
		void prepare(); // Need to be called in Render thread

		Ref<CubeTexture> getEnvMap();
		Ref<CubeTexture> getDiffuseMap();
		Ref<CubeTexture> getPrefilterMap();
		Ref<Texture2D>   getBrdfLUT();

		float getIntensity() const { return m_envMapIntensity; }
		AssetHandle getCubeMesh() const { return m_cubeMesh; }
		const std::string &getName() const { return m_image.Name; }

		void setIntensity(float val) { m_envMapIntensity = val; }

	private:
		void createData(const std::string &name, AssetFmt fmt);

		void createHDRPass();
		void createDiffuseConvPass();
		void createSpecularConvPass();
		void createBrdfConvPass();

		void createEnvMap();

	private:
		Image m_image{};
		Ref<Texture2D> m_HDRTexture;

		Ref<RenderPass> m_hdrPass;
		ColorAttachment m_envMapAttachment{ ColorAttachment::Num };
		glm::uvec2 m_envMapSize{ 512, 512 };

		Ref<RenderPass> m_diffuseConvPass;
		ColorAttachment m_diffuseMapAttachment{ ColorAttachment::Num }; // IBL irradiance convolution
		glm::uvec2 m_diffuseMapSize{ 32, 32 };

		Ref<RenderPass> m_prefilterMapConvPass;
		ColorAttachment m_prefilterMapAttachment{ ColorAttachment::Num }; // IBL specular convolution -- prefilter map
		glm::uvec2 m_prefilterSize{ 128, 128 };

		Ref<RenderPass> m_brdfConvPass;
		ColorAttachment m_brdfAttachment{ ColorAttachment::Num }; // IBL specular convolution -- brdf convolution
		glm::uvec2 m_brdfTextureSize{ 512, 512 };
		AssetHandle m_quad{ NullAsset };
		
		glm::mat4 m_prjMat{ 1.f };
		glm::mat4 m_viewMat[LightDir_Num];
		AssetHandle m_cubeMesh{ NullAsset };

		float m_envMapIntensity{ 0.1f };
	};
}

#endif