#include "Shader.h"
#include "ShaderCompiler.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/UniformData.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Shader"

namespace pio
{
	void Shader::SetUniformData(Ref<UniformData>& unimData)
	{
		UniformType type = unimData->Type();
		switch (type)
		{
			case UniformType::Bool:
			{
				bool val;
				unimData->read(&val);
				SetBool(unimData->Name(), val);
				break;
			}
			case UniformType::Float:
			{
				float val;
				unimData->read(&val);
				SetFloat(unimData->Name(), val);
				break;
			}
			case UniformType::UInt:
			{
				uint32_t val;
				unimData->read(&val);
				SetUInt(unimData->Name(), val);
				break;
			}
			case UniformType::Int:
			{
				int32_t val;
				unimData->read(&val);
				SetInt(unimData->Name(), val);
				break;
			}
			case UniformType::Mat2:
			{
				glm::mat2 val;
				unimData->read(&val);
				SetMat2(unimData->Name(), val);
				break;
			}
			case UniformType::Mat3:
			{
				glm::mat3 val;
				unimData->read(&val);
				SetMat3(unimData->Name(), val);
				break;
			}
			case UniformType::Mat4:
			{
				glm::mat4 val;
				unimData->read(&val);
				SetMat4(unimData->Name(), val);
				break;
			}
			case UniformType::Vec2:
			{
				glm::vec2 val;
				unimData->read(&val);
				SetVec2(unimData->Name(), val);
				break;
			}
			case UniformType::Vec3:
			{
				glm::vec3 val;
				unimData->read(&val);
				SetVec3(unimData->Name(), val);
				break;
			}
			case UniformType::Vec4:
			{
				glm::vec4 val;
				unimData->read(&val);
				SetVec4(unimData->Name(), val);
				break;
			}
			case UniformType::IVec2:
			{
				glm::ivec2 val;
				unimData->read(&val);
				SetIVec2(unimData->Name(), val);
				break;
			}
			case UniformType::IVec3:
			{
				glm::ivec3 val;
				unimData->read(&val);
				SetIVec3(unimData->Name(), val);
				break;
			}
			case UniformType::IVec4:
			{
				glm::ivec4 val;
				unimData->read(&val);
				SetIVec4(unimData->Name(), val);
				break;
			}
			default:
			{
				LOGE("err! uniform data[%s]'s type[%u] has been implemented", unimData->Name().c_str(), type);
				break;
			}
		}
	}

	Ref<ShaderMutant> Shader::MakeMutant(uint32_t flags)
	{
		if(flags == 0)
		{
			LOGE("err! permutation flag is 0");
			std::abort();
			return Ref<ShaderMutant>();
		}
		Shader* shader = Is<ShaderMutant>() ? As<ShaderMutant>()->m_Parent.lock().get() : this;
		auto result = shader->FindMutant(flags);
		return result;
	}

	Ref<ShaderMutant> Shader::FindMutant(uint32_t flags)
	{
		if(flags == 0)
		{
			LOGE("err! permutation flag is 0");
			std::abort();
			return Ref<ShaderMutant>();
		}
		auto it = m_Permutations.find(flags);
		if(it != m_Permutations.end())
		{
			return it->second;
		}
		ShaderBuilder builder = m_Builder;
		builder.SetMutant();

		ShaderUtils::AddMacros(flags, builder);

		auto context = m_Context.lock();
		auto mutant = RefCast<Shader, ShaderMutant>(ShaderCompiler::Compile(context, builder));
		mutant->m_Builder = builder;
		mutant->m_Parent = Self<Shader>();
		mutant->m_PermutationFlag = flags;

		if(context->IsRenderThread())
		{
			mutant->Init();
		}

		m_Permutations.insert({ flags, mutant });
		return mutant;
	}
}