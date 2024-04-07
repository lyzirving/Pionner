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
			createEnvMap();
		}
		else
		{
			LOGE("invalid image fmt");
		}
	}

	Ref<CubeTexture> Skybox::getEnvMap()
	{
		if (!m_hdrPass->getFramebuffer()->getColorBuffer(m_envMapAttachment)->isInit())
		{
			LOGE("prepare() has not been called");
			return Ref<CubeTexture>();
		}
		return RefCast<Texture2D, CubeTexture>(m_hdrPass->getFramebuffer()->getColorBuffer(m_envMapAttachment));
	}

	Ref<CubeTexture> Skybox::getDiffuseMap()
	{
		if (!m_convPass->getFramebuffer()->getColorBuffer(m_diffuseMapAttachment)->isInit())
		{
			LOGE("prepare() has not been called");
			return Ref<CubeTexture>();
		}
		return RefCast<Texture2D, CubeTexture>(m_convPass->getFramebuffer()->getColorBuffer(m_diffuseMapAttachment));
	}

	Ref<CubeTexture> Skybox::getPrefilterMap()
	{
		if (!m_convPass->getFramebuffer()->getColorBuffer(m_prefilterMapAttachment)->isInit())
		{
			LOGE("prepare() has not been called");
			return Ref<CubeTexture>();
		}
		return RefCast<Texture2D, CubeTexture>(m_convPass->getFramebuffer()->getColorBuffer(m_prefilterMapAttachment));
	}

	void Skybox::createData(const std::string &name, AssetFmt fmt)
	{
		ImageImporter importer(name, fmt);
		bool ret = importer.importToImage(m_image);
		LOGD("succeed[%s] to import image[%s]", ret ? "true" : "false", name.c_str());

		if (!ret)
			return;

		createHDRPass();
		createIblConvPass();
	}

	void Skybox::createHDRPass()
	{
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

		FrameBufferSpecification skyboxFboSpec;
		skyboxFboSpec.Name = "SkyboxFrameBuffer";
		skyboxFboSpec.Width = m_envMapSize.x;
		skyboxFboSpec.Height = m_envMapSize.y;

		TextureSpecification envMapSpec;
		envMapSpec.Name = "SkyboxEnvMap";
		envMapSpec.Format = m_image.Channel == 4 ? ImageInternalFormat::RGBA16F : ImageInternalFormat::RGB16F;
		envMapSpec.Width = m_envMapSize.x;
		envMapSpec.Height = m_envMapSize.y;
		envMapSpec.MinFilter = TextureFilterMin::Linear;
		envMapSpec.MaxFilter = TextureFilterMag::Linear;
		envMapSpec.WrapS = TextureWrap::ClampEdge;
		envMapSpec.WrapT = TextureWrap::ClampEdge;
		envMapSpec.WrapR = TextureWrap::ClampEdge;
		envMapSpec.AType = AssetType::CubeTexture;
		m_envMapAttachment = ColorAttachment(skyboxFboSpec.ColorBufferSpec.size());
		skyboxFboSpec.ColorBufferSpec.push_back(envMapSpec);

		TextureSpecification depthBufferSpec;
		depthBufferSpec.Name = "SkyboxDepthBuffer";
		depthBufferSpec.Format = ImageInternalFormat::DEPTH24;
		depthBufferSpec.Width = m_envMapSize.x;
		depthBufferSpec.Height = m_envMapSize.y;
		depthBufferSpec.AType = AssetType::RenderBuffer;

		skyboxFboSpec.FrameBufferUsage = FrameBufferUsage::ColorBuffer;
		skyboxFboSpec.DepthBufferSpec = depthBufferSpec;
		skyboxFboSpec.DepthAttachment = DepthAttachment::Depth;
		Ref<FrameBuffer> fbo = FrameBuffer::Create(skyboxFboSpec);

		RenderPassSpecification hdrPassSpec;
		hdrPassSpec.Name = "HDRPass";
		hdrPassSpec.FrameBuffer = fbo;
		m_hdrPass = RenderPass::Create(hdrPassSpec);

		RenderState state;
		state.Blend = Blend::Disable();
		state.Cull = CullFace::Common();
		state.DepthTest = DepthTest::Common();
		state.Clear = Clear::Common(Renderer::TRANSPARENT_COLOR);
		state.Stencil.Enable = false;
		m_hdrPass->setState(state);
	}

	void Skybox::createIblConvPass()
	{
		FrameBufferSpecification convPassFboSpec;
		convPassFboSpec.Name = "ConvPassFrameBuffer";
		convPassFboSpec.Width = m_diffuseMapSize.x;
		convPassFboSpec.Height = m_diffuseMapSize.y;

		TextureSpecification diffuseMapSpec;
		diffuseMapSpec.Name = "DiffuseEnvMap";
		diffuseMapSpec.Format = ImageInternalFormat::RGB16F;
		diffuseMapSpec.Width = m_diffuseMapSize.x;
		diffuseMapSpec.Height = m_diffuseMapSize.y;
		diffuseMapSpec.MinFilter = TextureFilterMin::Linear;
		diffuseMapSpec.MaxFilter = TextureFilterMag::Linear;
		diffuseMapSpec.WrapS = TextureWrap::ClampEdge;
		diffuseMapSpec.WrapT = TextureWrap::ClampEdge;
		diffuseMapSpec.WrapR = TextureWrap::ClampEdge;
		diffuseMapSpec.AType = AssetType::CubeTexture;
		m_diffuseMapAttachment = ColorAttachment(convPassFboSpec.ColorBufferSpec.size());
		convPassFboSpec.ColorBufferSpec.push_back(diffuseMapSpec);

		TextureSpecification prefilterMapSpec;
		prefilterMapSpec.Name = "PrefilterMap";
		prefilterMapSpec.Format = ImageInternalFormat::RGB16F;
		prefilterMapSpec.Width  = m_prefilterSize.x;
		prefilterMapSpec.Height = m_prefilterSize.y;
		prefilterMapSpec.MinFilter = TextureFilterMin::Linear_Mipmap_Linear;
		prefilterMapSpec.MaxFilter = TextureFilterMag::Linear;
		prefilterMapSpec.WrapS = TextureWrap::ClampEdge;
		prefilterMapSpec.WrapT = TextureWrap::ClampEdge;
		prefilterMapSpec.WrapR = TextureWrap::ClampEdge;
		prefilterMapSpec.AType = AssetType::CubeTexture;
		prefilterMapSpec.GenerateMips = true;
		prefilterMapSpec.MaxMipLevel = 5;
		m_prefilterMapAttachment = ColorAttachment(convPassFboSpec.ColorBufferSpec.size());
		convPassFboSpec.ColorBufferSpec.push_back(prefilterMapSpec);

		TextureSpecification depthBufferSpec;
		depthBufferSpec.Name = "ConvPassDepthBuffer";
		depthBufferSpec.Format = ImageInternalFormat::DEPTH24;
		depthBufferSpec.Width = m_diffuseMapSize.x;
		depthBufferSpec.Height = m_diffuseMapSize.y;
		depthBufferSpec.AType = AssetType::RenderBuffer;

		convPassFboSpec.FrameBufferUsage = FrameBufferUsage::ColorBuffer;
		convPassFboSpec.DepthBufferSpec = depthBufferSpec;
		convPassFboSpec.DepthAttachment = DepthAttachment::Depth;
		Ref<FrameBuffer> fbo = FrameBuffer::Create(convPassFboSpec);

		RenderPassSpecification convPassSpec;
		convPassSpec.Name = "ConvolutionPass";
		convPassSpec.FrameBuffer = fbo;
		m_convPass = RenderPass::Create(convPassSpec);

		RenderState state;
		state.Blend = Blend::Disable();
		state.Cull = CullFace::Common();
		state.DepthTest = DepthTest::Common();
		state.Clear = Clear::Common(Renderer::TRANSPARENT_COLOR);
		state.Stencil.Enable = false;
		m_convPass->setState(state);
	}

	void Skybox::createEnvMap()
	{
		PIO_ASSERT_RETURN(Renderer::IsRenderThread(), "Skybox::createEnvMap() should be called in render thread");
		m_HDRTexture->init();
		// --------- draw HDR raw pixel into cube texture --------------
		Renderer::BeginRenderPass(m_hdrPass);
		Ref<FrameBuffer> skyboxFbo = m_hdrPass->getFramebuffer();
		Ref<CubeTexture> envMap = RefCast<Texture2D, CubeTexture>(skyboxFbo->getColorBuffer(m_envMapAttachment));
		Renderer::RenderHDRToEnvMap(m_cubeMesh, 0, m_prjMat, m_viewMat, RenderState{}, m_envMapAttachment, m_HDRTexture, skyboxFbo);
		Renderer::EndRenderPass(m_hdrPass);
		// -------------------------------------------------------------

		Renderer::BeginRenderPass(m_convPass);
		Ref<FrameBuffer> convPassFbo = m_convPass->getFramebuffer();
		Renderer::RenderDiffuseConvolution(m_cubeMesh, 0, m_prjMat, m_viewMat, RenderState{}, m_diffuseMapAttachment, envMap, convPassFbo);
		Renderer::RenderPrefilterMapConvolution(m_cubeMesh, 0, m_prjMat, m_viewMat, RenderState{}, m_prefilterMapAttachment, envMap, convPassFbo);
		Renderer::EndRenderPass(m_convPass);
	}
}