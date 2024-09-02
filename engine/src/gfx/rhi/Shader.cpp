#include "Shader.h"

#include "scene/resources/Material.h"

#include "gfx/rhi/UniformData.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Shader"

namespace pio
{
	void Shader::setMaterial(Ref<Material>& material)
	{
		const auto& unims = material->uniforms();
		for (auto it : unims)
		{
			setUniformData(it.second);
		}
	}

	void Shader::setUniformData(Ref<UniformData>& unimData)
	{
		UniformDataType type = unimData->type();
		switch (type)
		{
			case UniformDataType::Bool:
			{
				bool val;
				unimData->read(&val);
				setBool(unimData->name(), val);
				break;
			}
			case UniformDataType::Float:
			{
				float val;
				unimData->read(&val);
				setFloat(unimData->name(), val);
				break;
			}
			case UniformDataType::UInt:
			{
				uint32_t val;
				unimData->read(&val);
				setUInt(unimData->name(), val);
				break;
			}
			case UniformDataType::Int:
			{
				int32_t val;
				unimData->read(&val);
				setInt(unimData->name(), val);
				break;
			}
			case UniformDataType::Mat2:
			{
				glm::mat2 val;
				unimData->read(&val);
				setMat2(unimData->name(), val);
				break;
			}
			case UniformDataType::Mat3:
			{
				glm::mat3 val;
				unimData->read(&val);
				setMat3(unimData->name(), val);
				break;
			}
			case UniformDataType::Mat4:
			{
				glm::mat4 val;
				unimData->read(&val);
				setMat4(unimData->name(), val);
				break;
			}
			case UniformDataType::Vec2:
			{
				glm::vec2 val;
				unimData->read(&val);
				setVec2(unimData->name(), val);
				break;
			}
			case UniformDataType::Vec3:
			{
				glm::vec3 val;
				unimData->read(&val);
				setVec3(unimData->name(), val);
				break;
			}
			case UniformDataType::Vec4:
			{
				glm::vec4 val;
				unimData->read(&val);
				setVec4(unimData->name(), val);
				break;
			}
			case UniformDataType::IVec2:
			{
				glm::ivec2 val;
				unimData->read(&val);
				setIVec2(unimData->name(), val);
				break;
			}
			case UniformDataType::IVec3:
			{
				glm::ivec3 val;
				unimData->read(&val);
				setIVec3(unimData->name(), val);
				break;
			}
			case UniformDataType::IVec4:
			{
				glm::ivec4 val;
				unimData->read(&val);
				setIVec4(unimData->name(), val);
				break;
			}
			default:
			{
				LOGE("err! uniform data[%s]'s type[%u] has been implemented", unimData->name().c_str(), type);
				break;
			}
		}
	}
}