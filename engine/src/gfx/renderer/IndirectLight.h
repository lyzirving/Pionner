#pragma once

#include "FrameBufferProxy.h"
#include "UBufferProxy.h"

namespace pio
{
	class Shader;
	class CubeMap;
	class Texture2D;
	class RenderContext;

	class UIndirectLightBuffer : public UBufferProxy
	{
		RTTR_ENABLE(UBufferProxy)
	public:
		UIndirectLightBuffer(const Ref<RenderContext>& context, const std::string& name);
		~UIndirectLightBuffer() = default;

		void SetValues(const glm::vec3(&sphericalHarmonics)[9]);

		PIO_DEFINE_PROPERTY(bool, Valid, false)
	};

	class IndirectLight : public FrameBufferProxy
	{
		RTTR_ENABLE(FrameBufferProxy)
	public:
		IndirectLight(const Ref<RenderContext>& context, const std::string& name);
		virtual ~IndirectLight() = default;

		void OnTick();

		void BindAt(const Ref<Shader>& shader);
		void UnBindAt();

		const Ref<UniformBuffer>& GetUBuffer() const { return m_UBuffer; }

	private:
		bool TryGetIblSpecular();
		void CheckValid();

	private:
		Ref<UniformBuffer> m_UBuffer;
		Ref<CubeMap> m_IblSpecular;
		Ref<Texture2D> m_IblDFG;		
		Ref<Texture2D> m_SSR;
	};
}