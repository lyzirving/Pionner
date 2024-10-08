#include "CascadeShadowMap.h"

#include "GlobalSettings.h"

#include "Camera.h"
#include "Frustum.h"
#include "ShadowMapFrameBuffer.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/resource/LightData.h"
#include "gfx/rhi/Texture.h"
#include "gfx/rhi/Shader.h"

#include "scene/Components.h"
#include "scene/node/LightNode.h"
#include "scene/node/CameraNode.h"

#include <limits>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "CascadeShadowMap"

namespace pio
{
	CascadeShadowMap::CascadeShadowMap(Ref<RenderContext>& context) : LightTechBase()
	{
		m_lightCam = CreateRef<Camera>();
		m_frameBuff = CreateRef<CascadeShadowFrameBuffer>(context);						

		m_UData = CreateRef<CascadeShadowMapUD>();
		m_UData->IndicateColor[0] = glm::vec4(1.f, 0.f, 0.f, 1.f);
		m_UData->IndicateColor[1] = glm::vec4(0.f, 1.f, 0.f, 1.f);
		m_UData->IndicateColor[2] = glm::vec4(0.f, 0.f, 1.f, 1.f);
		m_UBuffer = UniformBuffer::Create(context, m_UData->Block.getByteUsed(), UBBinding_DirectionalLightShadow, BufferUsage::Dynamic);

		m_attrs.setClear(Clear::Common())
			.setBlend(Blend::Disable())
			.setDepth(DepthTest::Common())					
			.setCull(CullFace::Create(FaceDirection::CouterClockwise, FaceMode_Front))
			.setStencil(StencilTest::Disable());
	}

	bool CascadeShadowMap::bind(Ref<Shader>& shader)
	{
		if (!shader)
			return false;

		shader->setBool("u_indicateCSM", GlobalSettings::RenderConfig.IndicateCSM);

		TextureSampler slot;
		auto& deps = m_frameBuff->depthBuffers();
		for (size_t i = 0; i < CASCADE_NUM; i++)
		{
			if (!deps[i] || !deps[i]->is<Texture2D>() || !shader->getSampler(slot))
			{
				LOGE("err! fail to bind dep texture at [%u]", i);
				continue;
			}
			auto* tex = deps[i]->as<Texture2D>();
			tex->active(slot);
			tex->bind(); 
			std::string name("u_cascadeShadowMap[");
			name.append(std::to_string(i)).append("]");
			shader->setTextureSampler(name, slot);
		}
		return true;
	}

	bool CascadeShadowMap::bindUnimBlock(Ref<RenderContext>& context, Ref<Shader>& shader)
	{
		if (context->bindUnimBlock(shader, m_UBuffer, GpuAttr::BINDING_CASCADE_SHADOW_MAP))
		{
			m_UBuffer->bind();
			return true;
		}
		return false;
	}

	void CascadeShadowMap::unbindUnimBlock()
	{
		m_UBuffer->unbind();
	}

	void CascadeShadowMap::update(Ref<RenderContext>& context, Ref<CameraNode>& camNode, Ref<DirectionalLightNode>& lightNode)
	{
		const auto& lightDir = lightNode->direction();
		glm::vec3 lightPos{ 0.f };
		m_lightCam->setPosition(lightPos);
		m_lightCam->setLookAt(lightPos + lightDir * 10.f);
		m_lightCam->setMotionMode(CameraMotionMode_FixTarget);
		m_lightCam->flush();
		auto lightMat = m_lightCam->viewMat();

		auto& cam = camNode->camera();
		glm::mat4 viewMatInv = cam->viewMat();
		viewMatInv = glm::inverse(viewMatInv);

		float aspect = cam->aspect();// width / height
		float fov = cam->fov();	
		float tanHalfFov = std::tan(glm::radians(fov * 0.5f));		
		float interval = (cam->frustFar() - cam->frustNear()) / float(CASCADE_NUM);
		m_cascadeEnds[0] = cam->frustNear();
		m_cascadeEnds[1] = m_cascadeEnds[0] + interval;
		m_cascadeEnds[2] = m_cascadeEnds[1] + interval;
		m_cascadeEnds[3] = cam->frustFar();			

		// Calculate each frustum's 8 corners to get AABB
		// Firstly, transform the frustum in View space to World space
		// Then, transform the point into Lighting space
		for (size_t i = 0; i < CASCADE_NUM; i++)
		{			
			float yn = m_cascadeEnds[i] * tanHalfFov;
			float xn = yn * aspect;

			m_lightFrustCorners[i].NearLb = viewMatInv * glm::vec4(-xn, -yn, -m_cascadeEnds[i], 1.f);//near left-bottom
			m_lightFrustCorners[i].NearLt = viewMatInv * glm::vec4(-xn, yn, -m_cascadeEnds[i], 1.f); //near left-top
			m_lightFrustCorners[i].NearRb = viewMatInv * glm::vec4(xn, -yn, -m_cascadeEnds[i], 1.f); //near right-bottom
			m_lightFrustCorners[i].NearRt = viewMatInv * glm::vec4(xn, yn, -m_cascadeEnds[i], 1.f);  //near right-top

			float yf = m_cascadeEnds[i + 1] * tanHalfFov;
			float xf = yf * aspect;

			m_lightFrustCorners[i].FarLb = viewMatInv * glm::vec4(-xf, -yf, -m_cascadeEnds[i + 1], 1.f);
			m_lightFrustCorners[i].FarLt = viewMatInv * glm::vec4(-xf, yf, -m_cascadeEnds[i + 1], 1.f);
			m_lightFrustCorners[i].FarRb = viewMatInv * glm::vec4(xf, -yf, -m_cascadeEnds[i + 1], 1.f);
			m_lightFrustCorners[i].FarRt = viewMatInv * glm::vec4(xf, yf, -m_cascadeEnds[i + 1], 1.f);

			glm::vec4 corners[8];
			corners[0] = lightMat * glm::vec4(m_lightFrustCorners[i].NearLb, 1.f);
			corners[1] = lightMat * glm::vec4(m_lightFrustCorners[i].NearLt, 1.f);
			corners[2] = lightMat * glm::vec4(m_lightFrustCorners[i].NearRb, 1.f);
			corners[3] = lightMat * glm::vec4(m_lightFrustCorners[i].NearRt, 1.f);

			corners[4] = lightMat * glm::vec4(m_lightFrustCorners[i].FarLb, 1.f);
			corners[5] = lightMat * glm::vec4(m_lightFrustCorners[i].FarLt, 1.f);
			corners[6] = lightMat * glm::vec4(m_lightFrustCorners[i].FarRb, 1.f);
			corners[7] = lightMat * glm::vec4(m_lightFrustCorners[i].FarRt, 1.f);

			glm::vec4 minVal(std::numeric_limits<float>::max()), maxVal(std::numeric_limits<float>::min());
			for (size_t j = 0; j < 8; j++)
			{
				minVal = glm::min(corners[j], minVal);
				maxVal = glm::max(corners[j], maxVal);
			}
			m_lightFrustums[i].setMax(maxVal);
			m_lightFrustums[i].setMin(minVal);

			m_UData->PrjMats[i] = Frustum::OrthoMat(minVal.x, maxVal.x, minVal.y, maxVal.y, minVal.z - SD_FRUST_EXT, maxVal.z + SD_FRUST_EXT);
			m_UData->ClipSpaceEnd[i] = (viewMatInv * glm::vec4(0.f, 0.f, -m_cascadeEnds[i + 1], 1.f)).z;
		}		
		m_UData->ViewMat = lightMat;
		m_UData->CascadeNum = CASCADE_NUM;
		m_UData->serialize();
		context->uploadData(m_UData->Block.getBuffer()->as<void*>(), m_UData->Block.getByteUsed(), m_UBuffer);
	}

	template<>
	bool Asset::is<CascadeShadowMap>() const
	{
		if (is<LightTechBase>())
		{
			return as<LightTechBase>()->lightTech() == LightTech::CascadeShadowMap;
		}
		return false;
	}
}