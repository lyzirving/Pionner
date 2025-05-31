#pragma once

#include "gfx/GfxDef.h"

namespace pio
{
	class Shader;
	class RenderContext;

	class ShaderLab : public Object
	{
		RTTR_ENABLE(Object)
	public:
		ShaderLab(const Ref<RenderContext>& context);
		~ShaderLab();

		Ref<Shader> Get(ShaderSpecifier spec);
		void Release();

	private:
		WeakRef<RenderContext> m_Context;
		std::unordered_map<uint8_t, Ref<Shader>> m_Shaders;
	};
}