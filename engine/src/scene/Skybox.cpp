#include "Skybox.h"

#include "asset/ImageImporter.h"
#include "asset/AssetsManager.h"

#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/RenderPass.h"

#include "gfx/struct/MeshFactory.h"
#include "gfx/renderer/Renderer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Skybox"

namespace pio
{
	Skybox::Skybox(const std::string &name, AssetFmt fmt)
	{
		m_cubeMesh = AssetsManager::CreateRuntimeAssets<StaticMesh>(MeshFactory::CreateCube())->getHandle();
		// set up projection and view matrices for capturing data onto the 6 cubemap face directions
		m_prjMat = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		m_viewMat[LightDir_PositiveX] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		m_viewMat[LightDir_NegativeX] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		m_viewMat[LightDir_PositiveY] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		m_viewMat[LightDir_NegativeY] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		m_viewMat[LightDir_NegativeZ] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		m_viewMat[LightDir_PositiveZ] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

		createData(name, fmt);
	}

	Skybox::~Skybox()
	{
		m_image.release();
	}

	void Skybox::prepare()
	{
		PIO_ASSERT_RETURN(Renderer::IsRenderThread(), "Skybox::prepare() should be called in render thread");
		if (m_image.AFmt == AssetFmt::HDR)
		{
			initHDR();
		}
		else
		{
			LOGE("invalid image fmt");
		}
	}

	Ref<CubeTexture> Skybox::getSkyBg()
	{
		if (!m_corePass->getFramebuffer()->getColorBuffer(m_cubeTexAttach)->isInit())
		{
			LOGE("prepare has not been called");
			return Ref<CubeTexture>();
		}
		return RefCast<Texture2D, CubeTexture>(m_corePass->getFramebuffer()->getColorBuffer(m_cubeTexAttach));
	}

	void Skybox::createData(const std::string &name, AssetFmt fmt)
	{
		ImageImporter imgImporter(name, fmt);
		bool ret = imgImporter.importToImage(m_image);
		LOGD("succeed[%s] to import image[%s]", ret ? "true" : "false", name.c_str());

		if (!ret)
			return;

		m_resolution.x = m_resolution.y = std::min(m_image.Width, m_image.Height);

		TextureSpecification HDRSpec;
		HDRSpec.Name = "HDRColorBuffer";
		HDRSpec.Format = m_image.Channel == 4 ? ImageInternalFormat::RGBA16F : ImageInternalFormat::RGB16F;
		HDRSpec.Width = m_image.Width;
		HDRSpec.Height = m_image.Height;
		HDRSpec.MinFilter = TextureFilterMin::Linear;
		HDRSpec.MaxFilter = TextureFilterMag::Linear;
		HDRSpec.WrapS = TextureWrap::ClampEdge;
		HDRSpec.WrapT = TextureWrap::ClampEdge;
		HDRSpec.AType = AssetType::Texture2D;
		Ref<Buffer> buffer = Texture::MakeBuffer(HDRSpec, m_image.Data<void *>());
		m_image.invalidate();
		m_HDRTexture = Texture2D::Create(HDRSpec, buffer);

		TextureSpecification cubeTextureSpec;
		cubeTextureSpec.Name = "SkyboxCubeTexture";
		cubeTextureSpec.Format = m_image.Channel == 4 ? ImageInternalFormat::RGBA16F : ImageInternalFormat::RGB16F;
		cubeTextureSpec.Width = m_resolution.x;
		cubeTextureSpec.Height = m_resolution.y;
		cubeTextureSpec.MinFilter = TextureFilterMin::Linear;
		cubeTextureSpec.MaxFilter = TextureFilterMag::Linear;
		cubeTextureSpec.WrapS = TextureWrap::ClampEdge;
		cubeTextureSpec.WrapT = TextureWrap::ClampEdge;
		cubeTextureSpec.WrapR = TextureWrap::ClampEdge;
		cubeTextureSpec.AType = AssetType::CubeTexture;

		TextureSpecification depthBufferSpec;
		depthBufferSpec.Name = "SkyboxDepthBuffer";
		depthBufferSpec.Format = ImageInternalFormat::DEPTH24;
		depthBufferSpec.Width = m_resolution.x;
		depthBufferSpec.Height = m_resolution.y;
		depthBufferSpec.AType = AssetType::RenderBuffer;

		FrameBufferSpecification cubeFboSpec;
		cubeFboSpec.Name = "SkyboxFrameBuffer";
		cubeFboSpec.Width = m_resolution.x;
		cubeFboSpec.Height = m_resolution.y;

		m_cubeTexAttach = ColorAttachment(cubeFboSpec.ColorBufferSpec.size());
		cubeFboSpec.ColorBufferSpec.push_back(cubeTextureSpec);
		cubeFboSpec.FrameBufferUsage = FrameBufferUsage::ColorBuffer;
		cubeFboSpec.DepthBufferSpec = depthBufferSpec;
		cubeFboSpec.DepthAttachment = DepthAttachment::Depth;
		Ref<FrameBuffer> fbo = FrameBuffer::Create(cubeFboSpec);

		RenderPassSpecification corePassSpec;
		corePassSpec.Name = "CorePass";
		corePassSpec.FrameBuffer = fbo;
		m_corePass = RenderPass::Create(corePassSpec);

		RenderState state;
		state.Blend = Blend::Disable();
		state.Cull = CullFace::Common();
		state.DepthTest = DepthTest::Common();
		state.Clear = Clear::Common(Renderer::TRANSPARENT_COLOR);
		state.Stencil.Enable = false;
		m_corePass->setState(state);
	}

	void Skybox::initHDR()
	{
		PIO_ASSERT_RETURN(Renderer::IsRenderThread(), "Skybox::initHDR() should be called in render thread");
		m_HDRTexture->init();

		// --------- draw HDR raw pixel into cube texture --------------
		Renderer::BeginRenderPass(m_corePass);
		Renderer::RenderHDRToCube(m_cubeMesh, 0, m_prjMat, m_viewMat, RenderState{}, m_cubeTexAttach, m_HDRTexture, m_corePass->getFramebuffer());
		Renderer::EndRenderPass(m_corePass);
		// -------------------------------------------------------------
	}
}