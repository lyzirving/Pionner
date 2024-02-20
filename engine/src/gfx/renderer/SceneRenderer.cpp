#include "SceneRenderer.h"

#include "Application.h"

#include "Renderer.h"

#include "scene/Scene.h"
#include "scene/TierSetting.h"
#include "scene/Skybox.h"

#include "gfx/struct/MeshUtil.h"
#include "gfx/struct/MaterialAsset.h"

#include "gfx/rhi/UniformBufferSet.h"
#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/RenderPass.h"

#include "gfx/debug/GDebugger.h"

#include "core/math/Transform.h"
#include "asset/AssetsManager.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "SceneRenderer"

namespace pio
{
	static void FillPointLightShadowData(const Camera &camera, const Ref<FrameBuffer> &fbo, LightEnvironment &lightEnv)
	{	
		float fov = 90.f;
		float frustumNear = camera.near();
		float frustumFar = camera.far();
		glm::mat4 prjMat = glm::perspective(glm::radians(fov), float(fbo->getWidth()) / float(fbo->getHeight()), frustumNear, frustumFar);
		for (uint32_t i = 0; i < lightEnv.PtLightShadowData.LightCount; i++)
		{
			PointLightMetadata &metadata = lightEnv.PtLightShadowData.Metadata[i];
			metadata.FrustumFar = frustumFar;
			metadata.Position = lightEnv.PointLightData[i].Position;
			metadata.LightSpaceMat[LightDir_PositiveX] = prjMat * glm::lookAt(metadata.Position, metadata.Position + glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f));
			metadata.LightSpaceMat[LightDir_NegativeX] = prjMat * glm::lookAt(metadata.Position, metadata.Position + glm::vec3(-1.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f));
			metadata.LightSpaceMat[LightDir_PositiveY] = prjMat * glm::lookAt(metadata.Position, metadata.Position + glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f));
			metadata.LightSpaceMat[LightDir_NegativeY] = prjMat * glm::lookAt(metadata.Position, metadata.Position + glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, -1.f));
			metadata.LightSpaceMat[LightDir_NegativeZ] = prjMat * glm::lookAt(metadata.Position, metadata.Position + glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, -1.f, 0.f));
			metadata.LightSpaceMat[LightDir_PositiveZ] = prjMat * glm::lookAt(metadata.Position, metadata.Position + glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, -1.f, 0.f));
		}
	}

	SceneRenderer::SceneRenderer()
	{
	}

	SceneRenderer::~SceneRenderer()
	{
	}

	void SceneRenderer::onAttach(const Scene &scene)
	{
		m_cameraUD.obtainBlock();
		const LightEnvironment &lightEnv = scene.m_lightEnv;
		Ref<Skybox> sk = scene.m_skybox;

		m_uniformBuffers = UniformBufferSet::Create();
		m_uniformBuffers->create(m_cameraUD.Block.getByteUsed(), PIO_UINT(UBBindings::Camera));
		m_uniformBuffers->create(lightEnv.DirectionalLight.Block.getByteUsed(), PIO_UINT(UBBindings::DistantLight));
		m_uniformBuffers->create(lightEnv.DirectionalLightShadowData.Block.getByteUsed(), PIO_UINT(UBBindings::DistantLightShadowData));
		m_uniformBuffers->create(lightEnv.PointLightData.Block.getByteUsed(), PIO_UINT(UBBindings::PointLightData));
		m_uniformBuffers->create(lightEnv.PtLightShadowData.Block.getByteUsed(), PIO_UINT(UBBindings::PointLightShadowData));

		uint32_t shadowWidth, shadowHeight;
		shadowWidth = shadowHeight = Tiering::GetShadowResolution(Tiering::ShadowResolutionSetting::Low);

		uint32_t colorWidth, colorHeight;
		colorWidth = Tiering::GetColorResolution(Tiering::ColorResolutionSetting::Low);
		colorHeight = colorWidth / Tiering::GetAspectRatio(Tiering::ColorAspectSetting::WH16_9);

		createShadowPass(shadowWidth, shadowHeight);
		createForwardPass(colorWidth, colorHeight);
		createDeferredPass(colorWidth, colorHeight);

		Renderer::SubmitTask([sk]() mutable
		{
			sk->prepare();
		});
	}

	void SceneRenderer::onDetach(const Scene &scene)
	{
	}

	void SceneRenderer::onRenderDestroy(const Scene &scene)
	{
		m_uniformBuffers.reset();
		m_GBufferPass.reset();
		m_lightPass.reset();
		m_distantLightShadowPass.reset();
		m_pointLightShadowPass.reset();
	}

	void SceneRenderer::beginScene(const Scene &scene, const Camera &camera)
	{
		m_active = true;

		LightEnvironment &lightEnv = const_cast<LightEnvironment &>(scene.m_lightEnv);
		CameraUD &cameraUD = m_cameraUD;
		Viewport &viewport = const_cast<Viewport &>(camera.getViewport());
		Camera &distLightCam = m_distantLightShadowPass->getCamera();

		distLightCam.setPosition(lightEnv.DirectionalLight.Position);
		distLightCam.setLookAt(lightEnv.DirectionalLight.Dest);	
		distLightCam.setOrtho(-10.f, 10.f, -10.f, 10.f);// TODO:
		distLightCam.setNearFar(camera.near(), camera.far());
		distLightCam.flush();

		cameraUD.ViewMat = camera.getViewMat();
		cameraUD.PrjMat = camera.getPrjMat();
		cameraUD.OrthoMat = camera.getOrthoMat();
		cameraUD.CameraPosition = camera.getCameraPos();
		cameraUD.FrustumFar = camera.far();		
		
		auto cameraUB = m_uniformBuffers->get((uint32_t)UBBindings::Camera);
		auto dirLightUB = m_uniformBuffers->get((uint32_t)UBBindings::DistantLight);
		auto dirLightSdDataUB = m_uniformBuffers->get((uint32_t)UBBindings::DistantLightShadowData);
		auto ptLightDataUB = m_uniformBuffers->get((uint32_t)UBBindings::PointLightData);
		auto ptLightSdDataUB = m_uniformBuffers->get((uint32_t)UBBindings::PointLightShadowData);

		lightEnv.DirectionalLightShadowData.ViewMat = distLightCam.getViewMat();
		lightEnv.DirectionalLightShadowData.PrjMat = distLightCam.getOrthoMat();

		FillPointLightShadowData(camera, m_pointLightShadowPass->getFramebuffer(), lightEnv);

		cameraUD.serialize();
		lightEnv.DirectionalLight.serialize();
		lightEnv.DirectionalLightShadowData.serialize();
		lightEnv.PointLightData.serialize();
		lightEnv.PtLightShadowData.serialize();		

		Renderer::SubmitRC([cameraUB, dirLightUB, dirLightSdDataUB, ptLightDataUB, ptLightSdDataUB, cameraUD, lightEnv, viewport]() mutable
		{
			cameraUB->setData(cameraUD.Block.getBuffer()->as<void *>(), cameraUD.Block.getByteUsed());
			dirLightUB->setData(lightEnv.DirectionalLight.Block.getBuffer()->as<void *>(), lightEnv.DirectionalLight.Block.getByteUsed());
			dirLightSdDataUB->setData(lightEnv.DirectionalLightShadowData.Block.getBuffer()->as<void *>(), lightEnv.DirectionalLightShadowData.Block.getByteUsed());
			ptLightDataUB->setData(lightEnv.PointLightData.Block.getBuffer()->as<void *>(), lightEnv.PointLightData.Block.getByteUsed());
			ptLightSdDataUB->setData(lightEnv.PtLightShadowData.Block.getBuffer()->as<void *>(), lightEnv.PtLightShadowData.Block.getByteUsed());
		});		
	}

	void SceneRenderer::endScene(const Scene &scene)
	{
		PIO_ASSERT_RETURN(m_active, "beginScene() has not been called!");

		flushDrawList(scene);

		m_meshDraws.clear();
		m_delayedMeshDraws.clear();
		m_shadowPassDraws.clear();
		m_spriteDraws.clear();

		m_active = false;
	}

	void SceneRenderer::submitMesh(Ref<MeshBase> &mesh, uint32_t submeshIndex, Transform &transform, const RenderState &state)
	{
		const auto &meshSource = mesh->getMeshSource();
		const auto &submeshes = meshSource->getSubmeshes();
		Ref<MaterialTable> mt = mesh->getMaterialTable();
		bool isRigged = submeshes[submeshIndex].IsRigged;

		MeshKey meshKey{ mesh->getHandle(), submeshIndex };

		// Main geo
		{
			auto &dc = m_meshDraws[meshKey];
			dc.Mesh = mesh->getHandle();
			dc.SubmeshIndex = submeshIndex;
			dc.MaterialTb = mt;
			// TODO: is the IsRigged correctly used?
			dc.ModelMat = isRigged ? transform.getMat() : transform.getMat() * submeshes[submeshIndex].Transform;
			dc.IsRigged = isRigged;
			dc.State = state;
		}

		// Shadow pass
		{
			AssetHandle handle = (*mt)[submeshIndex];
			if (NullAsset != handle && NullIndex != handle)
			{
				Ref<MaterialAsset> ma = AssetsManager::GetRuntimeAsset<MaterialAsset>(handle);
				if (ma && ma->isCastingShadow())
				{
					auto &dc = m_shadowPassDraws[meshKey];
					dc.Mesh = mesh->getHandle();
					dc.SubmeshIndex = submeshIndex;
					// TODO: is the IsRigged correctly used?
					dc.ModelMat = isRigged ? transform.getMat() : transform.getMat() * submeshes[submeshIndex].Transform;
					dc.IsRigged = isRigged;
					dc.State.DepthTest = DepthTest::Common();
					dc.State.Cull = CullFace::Create(FaceDirection::CouterClockwise, FaceMode_Front);
				}
			}
		}
	}

	void SceneRenderer::submitSprite(const AssetHandle &quadMesh, const AssetHandle &texture, const RenderState &state)
	{
		MeshKey meshKey{ quadMesh, 0 };

		{
			auto &dc = m_spriteDraws[meshKey];
			dc.QuadMesh = quadMesh;
			dc.Texture = texture;
			dc.State = state;
		}
	}

	void SceneRenderer::submitDelayedMesh(Ref<MeshBase> &mesh, uint32_t submeshIndex, Transform &transform, const RenderState &state)
	{
		const auto &meshSource = mesh->getMeshSource();
		const auto &submeshes = meshSource->getSubmeshes();
		Ref<MaterialTable> mt = mesh->getMaterialTable();

		MeshKey meshKey{ mesh->getHandle(), submeshIndex };

		{
			auto &dc = m_delayedMeshDraws[meshKey];
			dc.Mesh = mesh->getHandle();
			dc.SubmeshIndex = submeshIndex;
			dc.MaterialTb = mt;
			dc.ModelMat = transform.getMat() * submeshes[submeshIndex].Transform;
			dc.IsRigged = false;
			dc.State = state;
		}
	}

	void SceneRenderer::createForwardPass(uint32_t w, uint32_t h)
	{
		TextureSpecification colorBufSpec;
		colorBufSpec.Name = "ColorBuffer";
		colorBufSpec.Format = ImageInternalFormat::RGBA16F;
		colorBufSpec.Width = w;
		colorBufSpec.Height = h;
		colorBufSpec.MinFilter = TextureFilterMin::Nearest;
		colorBufSpec.MaxFilter = TextureFilterMag::Nearest;
		colorBufSpec.WrapS = TextureWrap::ClampEdge;
		colorBufSpec.WrapT = TextureWrap::ClampEdge;
		colorBufSpec.WrapR = TextureWrap::ClampEdge;
		colorBufSpec.AType = AssetType::Texture2D;

		// depth + stencil
		TextureSpecification depthBufferSpec;
		depthBufferSpec.Name = "DepthStencilBuffer";
		depthBufferSpec.Format = ImageInternalFormat::DEPTH24STENCIL8;
		depthBufferSpec.Width = w;
		depthBufferSpec.Height = h;
		depthBufferSpec.AType = AssetType::RenderBuffer;

		FrameBufferSpecification fpFboSpec;
		fpFboSpec.Name = "ForwardPassFrameBuffer";
		fpFboSpec.Width = w;
		fpFboSpec.Height = h;
		fpFboSpec.FrameBufferUsage = FrameBufferUsage::ColorBuffer;
		fpFboSpec.DepthBufferSpec = depthBufferSpec;
		fpFboSpec.DepthAttachment = DepthAttachment::DepthStencil;
		fpFboSpec.ColorBufferSpec.push_back(colorBufSpec);
		Ref<FrameBuffer> frameBuffer = FrameBuffer::Create(fpFboSpec);

		RenderPassSpecification fpSpec;
		fpSpec.Name = "ForwardPass";
		fpSpec.FrameBuffer = frameBuffer;
		m_forwardPass = RenderPass::Create(fpSpec);

		RenderState state;
		state.Blend = Blend::Disable();
		state.Cull = CullFace::Common();
		state.DepthTest = DepthTest::Common();
		state.Clear = Clear::Common(Renderer::TRANSPARENT_COLOR);
		state.Stencil.Enable = true;
		m_forwardPass->setState(state);
	}

	void SceneRenderer::createShadowPass(uint32_t w, uint32_t h)
	{
		// Directional light shadow pass
		{
			TextureSpecification depthBufferSpec;
			depthBufferSpec.Name = "DistantLightDepthTexture";
			depthBufferSpec.Format = ImageInternalFormat::DEPTH32F;
			depthBufferSpec.Width = w;
			depthBufferSpec.Height = h;
			depthBufferSpec.MinFilter = TextureFilterMin::Nearest;
			depthBufferSpec.MaxFilter = TextureFilterMag::Nearest;
			depthBufferSpec.WrapS = TextureWrap::ClampEdge;
			depthBufferSpec.WrapT = TextureWrap::ClampEdge;
			depthBufferSpec.WrapR = TextureWrap::ClampEdge;
			depthBufferSpec.Num = 1;
			depthBufferSpec.AType = AssetType::Texture2D;

			FrameBufferSpecification depthFboSpec;
			depthFboSpec.Name = "DistantLightDepthFrameBuffer";
			depthFboSpec.Width = w;
			depthFboSpec.Height = h;
			depthFboSpec.FrameBufferUsage = FrameBufferUsage::DepthBuffer;
			depthFboSpec.DepthBufferSpec = depthBufferSpec;
			depthFboSpec.DepthAttachment = DepthAttachment::Depth;
			Ref<FrameBuffer> depthFrameBuffer = FrameBuffer::Create(depthFboSpec);

			RenderPassSpecification shadowPassSpec;
			shadowPassSpec.Name = "DistantLightShadowPass";
			shadowPassSpec.FrameBuffer = depthFrameBuffer;
			m_distantLightShadowPass = RenderPass::Create(shadowPassSpec);

			RenderState state;
			// Fix Peter Panning
			state.Cull = CullFace::Create(FaceDirection::CouterClockwise, FaceMode_Front);
			state.Clear = Clear::Create(ClearBits::Depth);
			state.DepthTest = DepthTest::Common();
			state.Blend = Blend::Disable();
			m_distantLightShadowPass->setState(state);
		}

		// Point light shadow pass
		{
			TextureSpecification depthBufferSpec;
			depthBufferSpec.Name = "PointLightDepthTexture";
			depthBufferSpec.Format = ImageInternalFormat::DEPTH32F;
			depthBufferSpec.Width = w;
			depthBufferSpec.Height = h;
			depthBufferSpec.MinFilter = TextureFilterMin::Nearest;
			depthBufferSpec.MaxFilter = TextureFilterMag::Nearest;
			depthBufferSpec.WrapS = TextureWrap::ClampEdge;
			depthBufferSpec.WrapT = TextureWrap::ClampEdge;
			depthBufferSpec.WrapR = TextureWrap::ClampEdge;
			depthBufferSpec.Num = POINT_LIGHT_LIMIT;
			depthBufferSpec.AType = AssetType::CubeArrayTexture;

			FrameBufferSpecification depthFboSpec;
			depthFboSpec.Name = "PointLightDepthFrameBuffer";
			depthFboSpec.Width = w;
			depthFboSpec.Height = h;
			depthFboSpec.FrameBufferUsage = FrameBufferUsage::DepthBuffer;
			depthFboSpec.DepthBufferSpec = depthBufferSpec;
			depthFboSpec.DepthAttachment = DepthAttachment::Depth;
			Ref<FrameBuffer> depthFrameBuffer = FrameBuffer::Create(depthFboSpec);

			RenderPassSpecification shadowPassSpec;
			shadowPassSpec.Name = "PointLightShadowPass";
			shadowPassSpec.FrameBuffer = depthFrameBuffer;
			m_pointLightShadowPass = RenderPass::Create(shadowPassSpec);

			RenderState state;
			// Fix Peter Panning
			state.Cull = CullFace::Create(FaceDirection::CouterClockwise, FaceMode_Front);
			state.Clear = Clear::Create(ClearBits::Depth);
			state.DepthTest = DepthTest::Common();
			state.Blend = Blend::Disable();
			m_pointLightShadowPass->setState(state);
		}
	}

	void SceneRenderer::createDeferredPass(uint32_t w, uint32_t h)
	{
		createGBufferPass(w, h);
		createLightPass(w, h);
	}

	void SceneRenderer::createGBufferPass(uint32_t w, uint32_t h)
	{
		TextureSpecification positionBufSpec;
		positionBufSpec.Name = "GPositionColorBuffer";
		positionBufSpec.Format = ImageInternalFormat::RGBA16F;
		positionBufSpec.Width = w;
		positionBufSpec.Height = h;
		positionBufSpec.MinFilter = TextureFilterMin::Nearest;
		positionBufSpec.MaxFilter = TextureFilterMag::Nearest;
		positionBufSpec.WrapS = TextureWrap::ClampEdge;
		positionBufSpec.WrapT = TextureWrap::ClampEdge;
		positionBufSpec.WrapR = TextureWrap::ClampEdge;
		positionBufSpec.AType = AssetType::Texture2D;

		TextureSpecification normalBufSpec;
		normalBufSpec.Name = "GNormalBuffer";
		normalBufSpec.Format = ImageInternalFormat::RGBA16F;
		normalBufSpec.Width = w;
		normalBufSpec.Height = h;
		normalBufSpec.MinFilter = TextureFilterMin::Nearest;
		normalBufSpec.MaxFilter = TextureFilterMag::Nearest;
		normalBufSpec.WrapS = TextureWrap::ClampEdge;
		normalBufSpec.WrapT = TextureWrap::ClampEdge;
		normalBufSpec.WrapR = TextureWrap::ClampEdge;
		normalBufSpec.AType = AssetType::Texture2D;

		// abedo(3) + alpha(1)
		TextureSpecification colorBufSpec;
		colorBufSpec.Name = "GColorBuffer";
		colorBufSpec.Format = ImageInternalFormat::RGBA16F;
		colorBufSpec.Width = w;
		colorBufSpec.Height = h;
		colorBufSpec.MinFilter = TextureFilterMin::Nearest;
		colorBufSpec.MaxFilter = TextureFilterMag::Nearest;
		colorBufSpec.WrapS = TextureWrap::ClampEdge;
		colorBufSpec.WrapT = TextureWrap::ClampEdge;
		colorBufSpec.WrapR = TextureWrap::ClampEdge;
		colorBufSpec.AType = AssetType::Texture2D;

		// roughness(1) + metalness(1) + ao(1)
		TextureSpecification matBufSpec;
		matBufSpec.Name = "GMaterialBuffer";
		matBufSpec.Format = ImageInternalFormat::RGBA16F;
		matBufSpec.Width = w;
		matBufSpec.Height = h;
		matBufSpec.MinFilter = TextureFilterMin::Nearest;
		matBufSpec.MaxFilter = TextureFilterMag::Nearest;
		matBufSpec.WrapS = TextureWrap::ClampEdge;
		matBufSpec.WrapT = TextureWrap::ClampEdge;
		matBufSpec.WrapR = TextureWrap::ClampEdge;
		matBufSpec.AType = AssetType::Texture2D;

		// emission(3)
		TextureSpecification emissionBufSpec;
		emissionBufSpec.Name = "GEmissionBuffer";
		emissionBufSpec.Format = ImageInternalFormat::RGBA16F;
		emissionBufSpec.Width = w;
		emissionBufSpec.Height = h;
		emissionBufSpec.MinFilter = TextureFilterMin::Nearest;
		emissionBufSpec.MaxFilter = TextureFilterMag::Nearest;
		emissionBufSpec.WrapS = TextureWrap::ClampEdge;
		emissionBufSpec.WrapT = TextureWrap::ClampEdge;
		emissionBufSpec.WrapR = TextureWrap::ClampEdge;
		emissionBufSpec.AType = AssetType::Texture2D;

		// depth + stencil
		TextureSpecification depthBufferSpec;
		depthBufferSpec.Name = "GDepthStencilBuffer";
		depthBufferSpec.Format = ImageInternalFormat::DEPTH24STENCIL8;
		depthBufferSpec.Width = w;
		depthBufferSpec.Height = h;
		depthBufferSpec.AType = AssetType::RenderBuffer;

		FrameBufferSpecification gBufferSpec;
		gBufferSpec.Name = "GBuffer";
		gBufferSpec.Width = w;
		gBufferSpec.Height = h;

		gBufferSpec.ColorBufferSpec.reserve(5);
		gBufferSpec.ColorBufferSpec.push_back(positionBufSpec);
		gBufferSpec.ColorBufferSpec.push_back(normalBufSpec);
		gBufferSpec.ColorBufferSpec.push_back(colorBufSpec);
		gBufferSpec.ColorBufferSpec.push_back(matBufSpec);
		gBufferSpec.ColorBufferSpec.push_back(emissionBufSpec);

		gBufferSpec.FrameBufferUsage = FrameBufferUsage::ColorBuffer;
		gBufferSpec.DepthBufferSpec = depthBufferSpec;
		gBufferSpec.DepthAttachment = DepthAttachment::DepthStencil;
		Ref<FrameBuffer> gBuffer = FrameBuffer::Create(gBufferSpec);

		RenderPassSpecification gBufferPass;
		gBufferPass.Name = "GeometryPass";
		gBufferPass.FrameBuffer = gBuffer;
		m_GBufferPass = RenderPass::Create(gBufferPass);

		RenderState state;
		state.Blend = Blend::Disable();
		state.Cull = CullFace::Common();
		state.DepthTest = DepthTest::Common();
		state.Clear = Clear::Common(Renderer::TRANSPARENT_COLOR);
		state.Stencil.Enable = false;
		m_GBufferPass->setState(state);
	}

	void SceneRenderer::createLightPass(uint32_t w, uint32_t h)
	{
		TextureSpecification colorBufSpec;
		colorBufSpec.Name = "LightColorBuffer";
		colorBufSpec.Format = ImageInternalFormat::RGBA16F;
		colorBufSpec.Width = w;
		colorBufSpec.Height = h;
		colorBufSpec.MinFilter = TextureFilterMin::Nearest;
		colorBufSpec.MaxFilter = TextureFilterMag::Nearest;
		colorBufSpec.WrapS = TextureWrap::ClampEdge;
		colorBufSpec.WrapT = TextureWrap::ClampEdge;
		colorBufSpec.WrapR = TextureWrap::ClampEdge;
		colorBufSpec.AType = AssetType::Texture2D;

		TextureSpecification depthBufferSpec;
		depthBufferSpec.Name = "LightDepthStencilBuffer";
		depthBufferSpec.Format = ImageInternalFormat::DEPTH24STENCIL8;
		depthBufferSpec.Width = w;
		depthBufferSpec.Height = h;
		depthBufferSpec.AType = AssetType::RenderBuffer;

		FrameBufferSpecification lightingFboSpec;
		lightingFboSpec.Name = "LightingFrameBuffer";
		lightingFboSpec.Width = w;
		lightingFboSpec.Height = h;

		lightingFboSpec.ColorBufferSpec.push_back(colorBufSpec);
		lightingFboSpec.FrameBufferUsage = FrameBufferUsage::ColorBuffer;
		lightingFboSpec.DepthBufferSpec = depthBufferSpec;
		lightingFboSpec.DepthAttachment = DepthAttachment::DepthStencil;
		Ref<FrameBuffer> lightingFbo = FrameBuffer::Create(lightingFboSpec);

		RenderPassSpecification lightingPassSpec;
		lightingPassSpec.Name = "LightPass";
		lightingPassSpec.FrameBuffer = lightingFbo;
		m_lightPass = RenderPass::Create(lightingPassSpec);

		RenderState state;
		state.Blend = Blend::Disable();
		state.Cull = CullFace::Common();
		state.DepthTest = DepthTest::Common();
		state.Clear = Clear::Common(Renderer::TRANSPARENT_COLOR);
		state.Stencil.Enable = true;
		m_lightPass->setState(state);
	}

	void SceneRenderer::flushDrawList(const Scene &scene)
	{
		RenderStrategy strategy = Renderer::GetConfig().Strategy;
		switch (strategy)
		{
			case RenderStrategy::Forward:
			{
				forwardRendering(scene);
				break;
			}
			case RenderStrategy::Deferred:
			{
				deferredRendering(scene);
				break;
			}
			default:
				LOGE("render strategy[%u] is not implemented", strategy);
				break;
		}
	}

	void SceneRenderer::forwardRendering(const Scene &scene)
	{
		// TODO: support mutiple point lights in forward rendering
		return;

		auto &s = const_cast<RenderState &>(m_forwardPass->getState());
		s.Clear.Color = Renderer::GetConfig().ClearColor;	

		shadowPass(scene);
		geometryPass(scene);
		m_compositeTexture = m_forwardPass->getSpecification().FrameBuffer->getColorBuffer(ColorAttachment::Attach0);
	}

	void SceneRenderer::geometryPass(const Scene &scene)
	{
		Ref<RenderPass> fp = m_forwardPass;
		Ref<UniformBufferSet> ubs = m_uniformBuffers;
		// TODO: support mutiple point light in forward pass
		Ref<RenderPass> sp;
		std::map<MeshKey, DrawCommand> &cmd = m_meshDraws;

		Renderer::SubmitRC([fp, ubs, sp, cmd]() mutable
		{
			Renderer::BeginRenderPass(fp);
			for (auto &it : cmd)
			{
				auto &dc = it.second;
				Renderer::RenderSubmesh(dc.Mesh, dc.SubmeshIndex, dc.MaterialTb, sp, ubs, dc.ModelMat, dc.State);
			}

			// ---------- draw debug mesh -------------			
			GDebugger::Get()->flush();
			if (GDebugger::Get()->any(GDebugType::Line))
			{
				AssetHandle h = GDebugger::Get()->getLineMesh()->getHandle();
				Renderer::RenderLine(h, ubs, RenderState());
			}
			// ----------------------------------------

			Renderer::EndRenderPass(fp);
		});
	}

	void SceneRenderer::shadowPass(const Scene &scene)
	{
		const LightEnvironment &env = scene.m_lightEnv;
		Ref<RenderPass> distsp = m_distantLightShadowPass;
		Ref<RenderPass> ptsp = m_pointLightShadowPass;
		Ref<UniformBufferSet> ubs = m_uniformBuffers;
		std::map<MeshKey, DrawCommand> &cmd = m_shadowPassDraws;

		Renderer::SubmitRC([env, distsp, ubs, cmd]() mutable
		{
			Renderer::BeginRenderPass(distsp);
			for (auto &it : cmd)
			{
				auto &dc = it.second;
				Renderer::RenderDistantLightShadow(dc.Mesh, dc.SubmeshIndex, dc.IsRigged, dc.MaterialTb, distsp, ubs, dc.ModelMat, dc.State);
			}
			Renderer::EndRenderPass(distsp);
		});

		Renderer::SubmitRC([ptsp, ubs, cmd]() mutable
		{
			Renderer::BeginRenderPass(ptsp);
			for (auto &it : cmd)
			{
				auto &dc = it.second;
				Renderer::RenderPointLightShadow(dc.Mesh, dc.SubmeshIndex, dc.IsRigged, dc.MaterialTb, ptsp, ubs, dc.ModelMat, dc.State);
			}
			Renderer::EndRenderPass(ptsp);
		});
	}

	void SceneRenderer::deferredRendering(const Scene &scene)
	{
		shadowPass(scene);
		geometryPass_deferred(scene);
		lightingPass_deferred(scene);
		m_compositeTexture = m_lightPass->getFramebuffer()->getColorBuffer(ColorAttachment::Attach0);
	}

	void SceneRenderer::geometryPass_deferred(const Scene &scene)
	{
		Ref<RenderPass> gp = m_GBufferPass;
		Ref<UniformBufferSet> ubs = m_uniformBuffers;
		std::map<MeshKey, DrawCommand> &cmd = m_meshDraws;

		Renderer::SubmitRC([gp, ubs, cmd]() mutable
		{
			Renderer::BeginRenderPass(gp);
			for (auto &it : cmd)
			{
				auto &dc = it.second;
				Renderer::RenderSubmesh_Deferred(dc.Mesh, dc.SubmeshIndex, dc.MaterialTb, ubs, dc.ModelMat, dc.State);
			}
			Renderer::EndRenderPass(gp);
		});
	}

	void SceneRenderer::lightingPass_deferred(const Scene &scene)
	{
		const LightEnvironment &env = scene.m_lightEnv;
		Ref<Skybox> sk = scene.m_skybox;
		Ref<RenderPass> gp = m_GBufferPass;
		Ref<RenderPass> lp = m_lightPass;
		Ref<RenderPass> dlsp = m_distantLightShadowPass;
		Ref<RenderPass> plsp = m_pointLightShadowPass;
		Ref<UniformBufferSet> ubs = m_uniformBuffers;
		std::map<MeshKey, DrawCommand> &dCmd = m_delayedMeshDraws;
		std::map<MeshKey, SpriteCommand> &spriteCmd = m_spriteDraws;

		Renderer::SubmitRC([env, sk, gp, lp, dlsp, plsp, ubs, dCmd, spriteCmd]() mutable
		{						
			Renderer::BeginRenderPass(lp);
			// Copy depth buffer from G-Buffer into Lighting pass
			Renderer::FramebufferBlockTransfer(gp->getFramebuffer()->getId(), lp->getFramebuffer()->getId(),
											   glm::ivec2(0), glm::ivec2(gp->getFramebuffer()->getWidth(), gp->getFramebuffer()->getHeight()),
											   glm::ivec2(0), glm::ivec2(lp->getFramebuffer()->getWidth(), lp->getFramebuffer()->getHeight()),
											   FB_DepthBuffer_Bit, TextureFilterMag::Nearest);
			RenderPass::RenderLightingEffect_Deferred(env, gp, dlsp, plsp, ubs);

			// deferred mesh that should be drawn in forward shading at last
			for (auto &it : dCmd)
			{
				auto &dc = it.second;
				Renderer::RenderSubmesh(dc.Mesh, dc.SubmeshIndex, dc.MaterialTb, Ref<RenderPass>(), ubs, dc.ModelMat, dc.State);
			}

			RenderState skState;
			skState.Blend = Blend::Disable();
			skState.Cull = CullFace::Common();
			skState.DepthTest = DepthTest(FuncAttr::Lequal, DepthTest::Mask::ReadWrite);
			skState.Stencil.Enable = false;
			Renderer::RenderSkybox(sk->getCubeMesh(), 0, ubs, sk->getSkyBg(), skState);		

			RenderPass::RenderSprites(spriteCmd);

			Renderer::EndRenderPass(lp);	
		});
	}
}