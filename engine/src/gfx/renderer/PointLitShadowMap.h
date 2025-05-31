#pragma once

#include "FrameBufferProxy.h"

namespace pio
{
	class UniformBlock;
	class UniformBuffer;

	class PointLitShadowMap : public FrameBufferProxy
	{
		RTTR_ENABLE(FrameBufferProxy)
	public:
		PointLitShadowMap(const Ref<RenderContext>& context, const std::string& name);
		virtual ~PointLitShadowMap() = default;

		void SetLitPosition(int32_t index, const glm::vec3& position);
		void SetLitCastShadow(int32_t index, bool cast);
		void OnTick();

		Ref<UniformBuffer> GetUBuffer() { return m_UBuffer; }
		const Ref<UniformBuffer>& GetUBuffer() const { return m_UBuffer; }

	private:
		void CreateFrameBuff();
		void CreateUBuffer();		
		void CalcLitSpacePrjMat();
		void CalcLitSpaceMat(const glm::vec3& position);
		void InitAttrs();
		void SetLitSpaceMatData();

	protected:
		Ref<UniformBlock> m_UBlock;
		Ref<UniformBuffer> m_UBuffer;

		glm::mat4 m_PrjMat{ 1.f };
		glm::mat4 m_LitSpaceMat[CubeMapDir_Num];
		glm::vec3 m_LitPosition[PIO_POINT_LIT_LIMIT];

		PIO_DEFINE_PROPERTY(float, Fov, 90.f)
		PIO_DEFINE_PROPERTY(float, Near, 0.1f)
		PIO_DEFINE_PROPERTY(float, Far, 100.f)
		PIO_DEFINE_PROPERTY(int32_t, Num, PIO_POINT_LIT_LIMIT)
	};
}