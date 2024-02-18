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

	class Skybox
	{
	public:
		Skybox(const std::string &name, AssetFmt fmt);
		~Skybox();

	public:
		// Need to be called in Render thread
		void prepare();
		Ref<CubeTexture> getSkyBg();

		AssetHandle getCubeMesh() const { return m_cubeMesh; }

	private:
		void createData(const std::string &name, AssetFmt fmt);
		void initHDR();

	private:
		Image m_image{};
		Ref<Texture2D> m_HDRTexture;
		Ref<RenderPass> m_corePass;
		ColorAttachment m_cubeTexAttach{ ColorAttachment::Num };

		glm::uvec2 m_resolution{ 512, 512 };
		glm::mat4 m_prjMat{ 1.f };
		glm::mat4 m_viewMat[LightDir_Num];
		AssetHandle m_cubeMesh{ NullAsset };
	};
}

#endif