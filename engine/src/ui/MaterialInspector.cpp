#include "MaterialInspector.h"

#include "GlobalSettings.h"

#include "gfx/renderer/RenderContext.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/FrameBuffer.h"

#include "gfx/resource/Mesh.h"
#include "gfx/resource/LightData.h"
#include "gfx/resource/CameraData.h"
#include "gfx/resource/MeshRenderBuffer.h"
#include "gfx/resource/material/Material.h"

#include "scene/Factory.h"
#include "scene/3d/Camera.h"

#include "asset/AssetMgr.h"

namespace pio
{
	MaterialInspector* MaterialInspector::s_instance{ nullptr };
	std::mutex MaterialInspector::s_mutex{};

	void MaterialInspector::Init(const Ref<RenderContext>& context)
	{
		std::lock_guard<std::mutex> lk{ s_mutex };
		if (s_instance)
			delete s_instance;

		s_instance = new MaterialInspector(context);
	}

	void MaterialInspector::Shutdown()
	{
		std::lock_guard<std::mutex> lk{ s_mutex };
		delete s_instance;
		s_instance = nullptr;		
	}

	MaterialInspector::MaterialInspector(const Ref<RenderContext>& ctx) : m_context(ctx)
	{
		m_litData = CreateRef<DirectionalLightUD>();
		m_litBuff = UniformBuffer::Create(m_context, m_litData->Block.getByteUsed(), UBBinding_DirectionalLight, BufferUsage::Dynamic);
		
		m_litData->Color = glm::vec3(0.6f);
		m_litData->Intensity = 1.f;
		m_litData->Bias = 0.0001f;
		m_litData->NormalBias = 0.4f;
		m_litData->ShadowMode = ShadowMode_None;
		m_litData->CastShadow = false;
		m_litData->ShadowIntensity = 1.f;

		Transform3D transform;		
		transform.setEuler(glm::vec3(-45.f, -45.f, 0.f));
		m_litData->Direction = transform.rotMat() * glm::vec4(-World::Forward, 0.f);
		m_litData->Direction = glm::normalize(m_litData->Direction);
		m_litData->serialize();

		m_context->uploadData(m_litData->Block.getBuffer()->as<void*>(), m_litData->Block.getByteUsed(), m_litBuff);

		float radius = 1.f;
		auto meshData = Factory::MakeSphere(radius, 64);
		m_meshBuff = AssetMgr::MakeRuntimeAsset<MeshRenderBuffer>();
		m_meshBuff->setUp(m_context, meshData->getVertice(), meshData->getIndice());
		m_meshBuff->Transform.setPosition(glm::vec3(0.f));
		m_meshBuff->Transform.setEuler(glm::vec3(0.f));
		m_meshBuff->Transform.setScale(glm::vec3(1.f));
		m_meshBuff->onUpdate(m_context);

		// Camera is placed at (0, 0, cameraZ) to look at sphere located at (0, 0, 0);
		float cameraZ = 2.5f;
		m_camera = CreateRef<Camera>();
		m_camData = CreateRef<CameraUD>();
		m_camBuff = UniformBuffer::Create(m_context, m_camData->Block.getByteUsed(), UBBinding_Camera, BufferUsage::Dynamic);

		m_camera->setPosition(glm::vec3(0.f, 0.f, cameraZ));
		m_camera->setAspect(1.f);
		m_camera->setFov(60.f);
		m_camera->flush();

		m_camData->ViewMat = m_camera->viewMat();
		m_camData->PrjMat = m_camera->prjMat();
		m_camData->OrthoMat = m_camera->orthoMat();
		m_camData->CameraPosition = m_camera->position();
		m_camData->FrustumFar = m_camera->frustFar();
		m_camData->PrjType = m_camera->prjType();
		m_camData->serialize();
		m_context->uploadData(m_camData->Block.getBuffer()->as<void*>(), m_camData->Block.getByteUsed(), m_camBuff);

		auto colorSize = GlobalSettings::ColorResolution();
		m_w = m_h = std::min(colorSize.x, colorSize.y);
		FrameBufferSpecific fboSpec;
		fboSpec.Name = "MaterialInspectorFbo";
		fboSpec.Width = m_w;
		fboSpec.Height = m_h;
		PIO_FBO_ADD_USAGE(fboSpec.Usage, FrameBufferUsage_Color);

		TextureSpecificBuilder colorBuff;
		colorBuff.name("MaterialInspectorColorBuff")
			.type(TextureType::TwoDimen)
			.format(TextureFormat::RGBA_HALF)
			.width(m_w).height(m_h)
			.texWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
			.texFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);

		fboSpec.ColorSpec.push_back(colorBuff.build());

		m_frameBuff = FrameBuffer::Create(m_context, fboSpec);
		m_context->uploadData(m_frameBuff);
	}	

	Ref<Texture> MaterialInspector::colorBuffer() const
	{
		return m_frameBuff->colorBuffers()[0];
	}

	void MaterialInspector::onDraw()
	{
		if (!m_material || ShaderSpec_Standard != m_material->spec() || !bVisible())
		{
			return;
		}

		RenderStateAttrs attrs;
		attrs.setClear(Clear::Common())
			.setBlend(Blend::Disable())
			.setDepth(DepthTest::Disable())
			.setCull(CullFace::Common())
			.setStencil(StencilTest::Disable());

		m_context->onBeginFrameBuffer(m_frameBuff, attrs);

		auto shader = m_context->getShader(ShaderType::Forward);

		shader->bind();

		m_context->bindUnimBlock(shader, m_camBuff, GpuAttr::BINDING_CAM_BLOCK);
		m_camBuff->bind();

		m_context->bindUnimBlock(shader, m_litBuff, GpuAttr::BINDING_DIRECTIONAL_LIGHT_BLOCK);
		m_litBuff->bind();

		m_meshBuff->bind(shader);
		m_material->bind(shader);
		m_context->drawTriangles(m_meshBuff);

		m_litBuff->unbind();

		m_camBuff->unbind();

		shader->unbind();

		m_context->onEndFrameBuffer(m_frameBuff);
	}
}