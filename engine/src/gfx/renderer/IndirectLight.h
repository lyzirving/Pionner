#pragma once

#include "FrameBufferProxy.h"

namespace pio
{
	class Texture2D;
	class Shader;
	class IndirectLight : public FrameBufferProxy
	{
		RTTR_ENABLE(FrameBufferProxy)
	public:
		IndirectLight(const Ref<RenderContext>& context, const std::string& name);
		virtual ~IndirectLight() = default;

		void OnTick();
		void BindAt(const Ref<Shader>& shader);
		void UnBindAt();

	private:
		Ref<Texture2D> m_IblDFG;
		Ref<Texture2D> m_IblSpecular;		
		Ref<Texture2D> m_SSR;
	};
}