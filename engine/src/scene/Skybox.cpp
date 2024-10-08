#include "Skybox.h"

#include "asset/ImageImporter.h"
#include "asset/AssetsManager.h"

#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/RenderPass.h"

#include "gfx/struct/MeshFactory.h"
#include "gfx/struct/Geometry2D.h"
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
		if (!m_diffuseConvPass || !m_diffuseConvPass->getFramebuffer()->getColorBuffer(m_diffuseMapAttachment)->isInit())
		{
			LOGE("prepare() has not been called");
			return Ref<CubeTexture>();
		}
		return RefCast<Texture2D, CubeTexture>(m_diffuseConvPass->getFramebuffer()->getColorBuffer(m_diffuseMapAttachment));
	}

	Ref<CubeTexture> Skybox::getPrefilterMap()
	{
		if (!m_prefilterMapConvPass || !m_prefilterMapConvPass->getFramebuffer()->getColorBuffer(m_prefilterMapAttachment)->isInit())
		{
			LOGE("prepare() has not been called");
			return Ref<CubeTexture>();
		}
		return RefCast<Texture2D, CubeTexture>(m_prefilterMapConvPass->getFramebuffer()->getColorBuffer(m_prefilterMapAttachment));
	}

	Ref<Texture2D> Skybox::getBrdfLUT()
	{
		if (!m_brdfConvPass || !m_brdfConvPass->getFramebuffer()->getColorBuffer(m_brdfAttachment)->isInit())
		{
			LOGE("prepare() has not been called");
			return Ref<Texture2D>();
		}
		return m_brdfConvPass->getFramebuffer()->getColorBuffer(m_brdfAttachment);
	}

	void Skybox::createData(const std::string &name, AssetFmt fmt)
	{
		ImageImporter importer(name, fmt);
		bool ret = importer.importToImage(m_image);
		LOGD("succeed[%s] to import image[%s]", ret ? "true" : "false", name.c_str());

		if (!ret)
			return;

		createHDRPass();
		createDiffuseConvPass();
		createSpecularConvPass();
		createBrdfConvPass();
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
		envMapSpec.GenerateMips = true;
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

	void Skybox::createDiffuseConvPass()
	{
		FrameBufferSpecification diffuseConvPassFboSpec;
		diffuseConvPassFboSpec.Name = "DiffuseConvPassFbo";
		diffuseConvPassFboSpec.Width  = m_diffuseMapSize.x;
		diffuseConvPassFboSpec.Height = m_diffuseMapSize.y;

		TextureSpecification diffuseMapSpec;
		diffuseMapSpec.Name = "DiffuseEnvMap";
		diffuseMapSpec.Format = ImageInternalFormat::RGB16F;
		diffuseMapSpec.Width  = m_diffuseMapSize.x;
		diffuseMapSpec.Height = m_diffuseMapSize.y;
		diffuseMapSpec.MinFilter = TextureFilterMin::Linear;
		diffuseMapSpec.MaxFilter = TextureFilterMag::Linear;
		diffuseMapSpec.WrapS = TextureWrap::ClampEdge;
		diffuseMapSpec.WrapT = TextureWrap::ClampEdge;
		diffuseMapSpec.WrapR = TextureWrap::ClampEdge;
		diffuseMapSpec.AType = AssetType::CubeTexture;
		m_diffuseMapAttachment = ColorAttachment(diffuseConvPassFboSpec.ColorBufferSpec.size());
		diffuseConvPassFboSpec.ColorBufferSpec.push_back(diffuseMapSpec);

		TextureSpecification depthBufferSpec;
		depthBufferSpec.Name = "DiffuseConvPassDepthBuffer";
		depthBufferSpec.Format = ImageInternalFormat::DEPTH24;
		depthBufferSpec.Width  = m_diffuseMapSize.x;
		depthBufferSpec.Height = m_diffuseMapSize.y;
		depthBufferSpec.AType = AssetType::RenderBuffer;

		diffuseConvPassFboSpec.FrameBufferUsage = FrameBufferUsage::ColorBuffer;
		diffuseConvPassFboSpec.DepthBufferSpec = depthBufferSpec;
		diffuseConvPassFboSpec.DepthAttachment = DepthAttachment::Depth;
		Ref<FrameBuffer> fbo = FrameBuffer::Create(diffuseConvPassFboSpec);

		RenderPassSpecification convPassSpec;
		convPassSpec.Name = "DiffuseConvPass";
		convPassSpec.FrameBuffer = fbo;
		m_diffuseConvPass = RenderPass::Create(convPassSpec);

		RenderState state;
		state.Blend = Blend::Disable();
		state.Cull = CullFace::Common();
		state.DepthTest = DepthTest::Common();
		state.Clear = Clear::Common(Renderer::TRANSPARENT_COLOR);
		state.Stencil.Enable = false;
		m_diffuseConvPass->setState(state);
	}

	void Skybox::createSpecularConvPass()
	{
		FrameBufferSpecification prefilterConvPassFboSpec;
		prefilterConvPassFboSpec.Name = "PrefilterConvPassFbo";
		prefilterConvPassFboSpec.Width  = m_prefilterSize.x;
		prefilterConvPassFboSpec.Height = m_prefilterSize.y;

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
		m_prefilterMapAttachment = ColorAttachment(prefilterConvPassFboSpec.ColorBufferSpec.size());
		prefilterConvPassFboSpec.ColorBufferSpec.push_back(prefilterMapSpec);

		TextureSpecification depthBufferSpec;
		depthBufferSpec.Name = "PrefilterMapConvPassDepthBuffer";
		depthBufferSpec.Format = ImageInternalFormat::DEPTH24;
		depthBufferSpec.Width  = m_prefilterSize.x;
		depthBufferSpec.Height = m_prefilterSize.y;
		depthBufferSpec.AType = AssetType::RenderBuffer;

		prefilterConvPassFboSpec.FrameBufferUsage = FrameBufferUsage::ColorBuffer;
		prefilterConvPassFboSpec.DepthBufferSpec = depthBufferSpec;
		prefilterConvPassFboSpec.DepthAttachment = DepthAttachment::Depth;
		Ref<FrameBuffer> fbo = FrameBuffer::Create(prefilterConvPassFboSpec);

		RenderPassSpecification convPassSpec;
		convPassSpec.Name = "PrefilterMapConvPass";
		convPassSpec.FrameBuffer = fbo;
		m_prefilterMapConvPass = RenderPass::Create(convPassSpec);

		RenderState state;
		state.Blend = Blend::Disable();
		state.Cull = CullFace::Common();
		state.DepthTest = DepthTest::Common();
		state.Clear = Clear::Common(Renderer::TRANSPARENT_COLOR);
		state.Stencil.Enable = false;
		m_prefilterMapConvPass->setState(state);
	}

	void Skybox::createBrdfConvPass()
	{
		m_quad = MeshFactory::CreateScreenQuad(0, 0, m_brdfTextureSize.x, m_brdfTextureSize.y, m_brdfTextureSize.x, m_brdfTextureSize.y)->getHandle();

		FrameBufferSpecification brdfConvFboSpec;
		brdfConvFboSpec.Name = "BrdfConvPassFbo";
		brdfConvFboSpec.Width  = m_brdfTextureSize.x;
		brdfConvFboSpec.Height = m_brdfTextureSize.y;

		TextureSpecification brdfTexSpec;
		brdfTexSpec.Name = "BrdfConvTexture";
		brdfTexSpec.Format = ImageInternalFormat::RGB16F;
		brdfTexSpec.Width  = m_brdfTextureSize.x;
		brdfTexSpec.Height = m_brdfTextureSize.y;
		brdfTexSpec.MinFilter = TextureFilterMin::Linear;
		brdfTexSpec.MaxFilter = TextureFilterMag::Linear;
		brdfTexSpec.WrapS = TextureWrap::ClampEdge;
		brdfTexSpec.WrapT = TextureWrap::ClampEdge;
		brdfTexSpec.AType = AssetType::Texture2D;
		m_brdfAttachment = ColorAttachment(brdfConvFboSpec.ColorBufferSpec.size());
		brdfConvFboSpec.ColorBufferSpec.push_back(brdfTexSpec);

		TextureSpecification depthBufferSpec;
		depthBufferSpec.Name = "BrdfConvPassDepthBuffer";
		depthBufferSpec.Format = ImageInternalFormat::DEPTH24;
		depthBufferSpec.Width  = m_brdfTextureSize.x;
		depthBufferSpec.Height = m_brdfTextureSize.y;
		depthBufferSpec.AType = AssetType::RenderBuffer;

		brdfConvFboSpec.FrameBufferUsage = FrameBufferUsage::ColorBuffer;
		brdfConvFboSpec.DepthBufferSpec = depthBufferSpec;
		brdfConvFboSpec.DepthAttachment = DepthAttachment::Depth;
		Ref<FrameBuffer> fbo = FrameBuffer::Create(brdfConvFboSpec);

		RenderPassSpecification convPassSpec;
		convPassSpec.Name = "BrdfConvPass";
		convPassSpec.FrameBuffer = fbo;
		m_brdfConvPass = RenderPass::Create(convPassSpec);

		RenderState state;
		state.Blend = Blend::Disable();
		state.Cull = CullFace::Common();
		state.DepthTest = DepthTest::Common();
		state.Clear = Clear::Common(Renderer::TRANSPARENT_COLOR);
		state.Stencil.Enable = false;
		m_brdfConvPass->setState(state);
	}

	void Skybox::createEnvMap()
	{
		PIO_ASSERT_RETURN(Renderer::IsRenderThread(), "Skybox::createEnvMap() should be called in render thread");
		m_HDRTexture->init();

		Renderer::BeginRenderPass(m_hdrPass);
		Ref<FrameBuffer> skyboxFbo = m_hdrPass->getFramebuffer();
		Ref<CubeTexture> envMap = RefCast<Texture2D, CubeTexture>(skyboxFbo->getColorBuffer(m_envMapAttachment));
		Renderer::RenderHDRToEnvMap(m_cubeMesh, 0, m_prjMat, m_viewMat, RenderState{}, m_envMapAttachment, m_HDRTexture, skyboxFbo);
		Renderer::EndRenderPass(m_hdrPass);

		Renderer::BeginRenderPass(m_diffuseConvPass);
		Renderer::RenderDiffuseCnvl(m_cubeMesh, 0, m_prjMat, m_viewMat, RenderState{}, m_diffuseMapAttachment, envMap, m_diffuseConvPass->getFramebuffer());
		Renderer::EndRenderPass(m_diffuseConvPass);

		Renderer::BeginRenderPass(m_prefilterMapConvPass);
		Renderer::RenderDiffuseCnvl(m_cubeMesh, 0, m_prjMat, m_viewMat, RenderState{}, m_prefilterMapAttachment, envMap, m_prefilterMapConvPass->getFramebuffer());
		Renderer::EndRenderPass(m_prefilterMapConvPass);

		Renderer::BeginRenderPass(m_brdfConvPass);		
		Renderer::RenderBrdfCnvl(m_quad, RenderState{}, m_brdfConvPass->getFramebuffer());
		Renderer::EndRenderPass(m_brdfConvPass);
	}
}