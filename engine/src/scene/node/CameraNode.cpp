#include "CameraNode.h"

#include "GlobalSettings.h"
#include "asset/AssetMgr.h"

#include "scene/Components.h"
#include "scene/3d/Camera.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/resource/CameraData.h"
#include "gfx/resource/RenderTarget.h"
#include "gfx/rhi/FrameBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "CameraNode"

namespace pio
{
	CameraNode::CameraNode(Ref<RenderContext>& context, const entt::entity& key, entt::registry& regi, const std::string& name) : Node(context, key, regi, name)
	{
		m_camera = AssetMgr::MakeRuntimeAsset<Camera>();
		m_UData = CreateRef<CameraUD>();
		m_UBuffer = UniformBuffer::Create(context, m_UData->Block.getByteUsed(), UBBinding_Camera, BufferUsage::Dynamic);

		auto colorSize = GlobalSettings::ColorResolution();
		auto depthSize = GlobalSettings::ShadowResolution();
		{
			//Render target			
			FrameBufferSpecific fboSpec;
			fboSpec.Name = name + "-target";
			fboSpec.Width = colorSize.x;
			fboSpec.Height = colorSize.y;
			PIO_FBO_ADD_USAGE(fboSpec.Usage, FrameBufferUsage_Color);

			TextureSpecificBuilder texBuilder;
			texBuilder.name(fboSpec.Name + "-texture")
				.type(TextureType::TwoDimen)
				.format(TextureFormat::RGBA_HALF)
				.width(colorSize.x).height(colorSize.y)
				.texWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
				.texFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);

			TextureSpecificBuilder depthBuilder;
			depthBuilder.name(fboSpec.Name + "-depthStencil")
				.type(TextureType::RenderBuffer)
				.format(TextureFormat::DEPTH_24_STENCIL_8)
				.width(depthSize.x).height(depthSize.y);

			fboSpec.ColorSpec.push_back(texBuilder.build());
			fboSpec.DepthSpec = depthBuilder.build();

			m_target = RenderTarget::Create(context, fboSpec);
		}

		addComponent<CameraComponent, TransformComponent>();
		auto* camComp = getComponent<CameraComponent>();
		auto* transComp = getComponent<TransformComponent>();

		//Default value
		camComp->Aspect = float(colorSize.x) / float(colorSize.y);
		camComp->Uid = m_camera->assetHnd();
		transComp->Position = glm::vec3(0.f, 3.f, 4.5f);
		transComp->Rotation = glm::vec3(-35.f, 0.f, 0.f);
	}

	CameraNode::~CameraNode() = default;

	void CameraNode::update()
	{
		auto* camComp = getComponent<CameraComponent>();
		auto* tranComp = getComponent<TransformComponent>();

		m_camera->setPrjType(camComp->PrjType);
		m_camera->setFov(camComp->Fov);
		m_camera->setSize(camComp->Size);
		m_camera->setAspect(camComp->Aspect);
		m_camera->setNear(camComp->Near);
		m_camera->setFar(camComp->Far);
		m_camera->setPosition(tranComp->Position);
		m_camera->setEuler(tranComp->Rotation);

		if (m_camera->anyChange())
		{
			m_camera->flush();
			m_UData->ViewMat = m_camera->viewMat();
			m_UData->PrjMat = m_camera->prjMat();
			m_UData->OrthoMat = m_camera->orthoMat();
			m_UData->CameraPosition = m_camera->position();
			m_UData->FrustumFar = m_camera->frustFar();
			m_UData->PrjType = m_camera->prjType();
			m_UData->serialize();

			auto context = m_context.lock();
			if (context)
			{
				context->uploadData(m_UData->Block.getBuffer()->as<void*>(), m_UData->Block.getByteUsed(), m_UBuffer);
			}
			else
			{
				LOGE("err! context is invalid");
			}
		}
	}

	template<>
	bool Node::is<CameraNode>() const { return nodeType() == NodeType::Camera; }

	template<>
	Ref<UniformBuffer> Node::getRenderingData<CameraNode>() const
	{
		if (is<CameraNode>())
		{
			return as<CameraNode>()->m_UBuffer;
		}
		return Ref<UniformBuffer>();
	}
}