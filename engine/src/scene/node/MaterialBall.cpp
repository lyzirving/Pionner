#include "MaterialBall.h"

#include "GlobalSettings.h"

#include "component/CameraComponent.h"
#include "component/DirectionalLitComponent.h"
#include "component/StaticMeshComponent.h"
#include "component/TransformComponent.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/renderer/MeshRenderBuffer.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/UniformBuffer.h"
#include "gfx/rhi/UniformData.h"

#include "scene/Scene.h"
#include "asset/material/Material.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "MaterialBall"

namespace pio
{
	MaterialBall::MaterialBall() : SphereNode()
	{
	}

	void MaterialBall::OnInit()
	{
		SphereNode::OnInit();
		SetScale(glm::vec3(2.f));

		auto context = m_Context.lock();
		OnTickComponents(context);

		auto colorSize = GlobalSettings::ColorResolution();
		auto depthSize = GlobalSettings::ShadowResolution();

		FrameBufferSpecific fboSpec;
		fboSpec.Name = std::string(m_Name).append("_FrameBuffer");
		fboSpec.Width = colorSize.x;
		fboSpec.Height = colorSize.y;
		PIO_FBO_ADD_USAGE(fboSpec.Usage, FrameBufferUsage_Color);

		TextureSpecificBuilder colorBufferSpec;
		colorBufferSpec.Name(std::string(fboSpec.Name).append("_ColorBuffer"))
			.Type(TextureType::TwoDimen)
			.Format(TextureFormat::RGBA_HALF)
			.Width(colorSize.x).Height(colorSize.y)
			.TexWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
			.TexFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);

		fboSpec.ColorSpec.push_back(colorBufferSpec.Build());
		m_FrameBuff = FrameBuffer::Create(context, fboSpec);
		context->UploadData(m_FrameBuff);

		m_Attrs.SetClear(Clear::Common())
			.SetBlend(Blend::Disable())
			.SetDepth(DepthTest::Common())
			.SetCull(CullFace::Common())
			.SetStencil(StencilTest::Disable());

		auto camComp = AddComponent<CameraComponent>();
		camComp->SetPosition(glm::vec3(0.f, 0.f, 5.f));
		camComp->SetAspect(float(colorSize.x) / float(colorSize.y));
		camComp->OnTick();

		auto dirLitComp = AddComponent<DirectionalLitComponent>();
		Rotator rotator(-45.f, 0.f, 40.f);
		dirLitComp->SetDirection(glm::normalize(rotator.Mat() * (glm::vec4(-World::Forward, 0.f))));
		dirLitComp->SetbCastShadow(false);
		dirLitComp->OnTick();
	}

	void MaterialBall::SetMaterial(const Ref<Material>& material)
	{
		m_Material = material;

		auto comp = GetComponent<StaticMeshComponent>();
		auto& elem = comp->GetMaterialElems();
		if(!elem.empty())
		{
			elem[0] = m_Material;
		}
	}

	void MaterialBall::OnDraw()
	{
		if(!m_Material)
			return;

		auto context = m_Context.lock();
		auto camBuff = GetComponent<CameraComponent>()->GetUBuffer();
		auto dirLitBuff = GetComponent<DirectionalLitComponent>()->GetUBuffer();
		auto shader = context->FindShader(SHADER_SPEC_FORWARD);

		auto transComp = GetComponent<TransformComponent>();
		auto meshComp = GetComponent<StaticMeshComponent>();

		transComp->OnTick();
		meshComp->OnTick();

		const auto& meshBuffs = meshComp->GetRenderBuffer();
		const auto& uBuffers = meshComp->GetMotionBuffer();

		context->OnBeginFrameBuffer(m_FrameBuff, m_Attrs);
		shader->Bind();

		camBuff->BindBlock(context, shader);
		dirLitBuff->BindBlock(context, shader);
		m_Material->Bind(shader);

		for(size_t i = 0; i < meshBuffs.size(); i++)
		{
			uBuffers[i]->BindBlock(context, shader);
			context->DrawTriangles(meshBuffs[i]);
			uBuffers[i]->UnBind();
		}

		m_Material->UnBind();
		dirLitBuff->UnBind();
		camBuff->UnBind();
		shader->UnBind();
		context->OnEndFrameBuffer(m_FrameBuff);
	}

	float MaterialBall::GetAspect() const
	{
		return GetComponent<CameraComponent>()->GetAspect();
	}

	float MaterialBall::GetColorBufferId() const
	{
		return m_FrameBuff->ColorBuffers()[0]->Id();
	}

}