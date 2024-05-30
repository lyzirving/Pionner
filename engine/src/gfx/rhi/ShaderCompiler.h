#ifndef __PIONNER_GFX_RHI_SHADER_COMPILER_H__
#define __PIONNER_GFX_RHI_SHADER_COMPILER_H__

#include "RhiDef.h"

namespace pio
{
	class Shader;

	class ShaderCompiler
	{
	public:
		virtual ~ShaderCompiler() = default;

		virtual bool preprocess() = 0;

	public:
		static Ref<ShaderCompiler> Create(const Ref<Shader> &shader);
	};
}

#endif