#include "GLRenderPass.h"
#include "GLHelper.h"

#include "gfx/rhi/FrameBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLRenderPass"

namespace pio
{
	GLRenderPass::GLRenderPass(const RenderPassSpecification &spec) : RenderPass(), m_spec(spec)
	{
	}

	GLRenderPass::~GLRenderPass()
	{
		m_spec.FrameBuffer.reset();
	}

	void GLRenderPass::set(const std::string &name, float value)
	{
		set<float>(name, GLUniformType::Float, value);
	}

	void GLRenderPass::set(const std::string &name, int32_t value)
	{
		set<int32_t>(name, GLUniformType::Int, value);
	}

	void GLRenderPass::set(const std::string &name, uint32_t value)
	{
		set<uint32_t>(name, GLUniformType::UInt, value);
	}

	void GLRenderPass::set(const std::string &name, bool value)
	{
		set<bool>(name, GLUniformType::Bool, value);
	}

	void GLRenderPass::set(const std::string &name, const glm::vec2 &value)
	{
		set<glm::vec2>(name, GLUniformType::Vec2, value);
	}

	void GLRenderPass::set(const std::string &name, const glm::vec3 &value)
	{
		set<glm::vec3>(name, GLUniformType::Vec3, value);
	}

	void GLRenderPass::set(const std::string &name, const glm::vec4 &value)
	{
		set<glm::vec4>(name, GLUniformType::Vec4, value);
	}

	void GLRenderPass::set(const std::string &name, const glm::ivec2 &value)
	{
		set<glm::ivec2>(name, GLUniformType::IVec2, value);
	}

	void GLRenderPass::set(const std::string &name, const glm::ivec3 &value)
	{
		set<glm::ivec3>(name, GLUniformType::IVec3, value);
	}

	void GLRenderPass::set(const std::string &name, const glm::ivec4 &value)
	{
		set<glm::ivec4>(name, GLUniformType::IVec4, value);
	}

	void GLRenderPass::set(const std::string &name, const glm::mat3 &value)
	{
		set<glm::mat3>(name, GLUniformType::Mat3, value);
	}

	void GLRenderPass::set(const std::string &name, const glm::mat4 &value)
	{
		set<glm::mat4>(name, GLUniformType::Mat4, value);
	}

	float GLRenderPass::getFloat(const std::string &name)
	{
		float out;
		if (getValue<float>(name, GLUniformType::Float, out))
		{
			return out;
		}
		return 0.f;
	}

	int32_t GLRenderPass::getInt(const std::string &name)
	{
		int32_t out;
		if (getValue<int32_t>(name, GLUniformType::Int, out))
		{
			return out;
		}
		return 0;
	}

	uint32_t GLRenderPass::getUInt(const std::string &name)
	{
		uint32_t out;
		if (getValue<uint32_t>(name, GLUniformType::UInt, out))
		{
			return out;
		}
		return 0;
	}

	bool GLRenderPass::getBool(const std::string &name)
	{
		bool out;
		if (getValue<bool>(name, GLUniformType::Bool, out))
		{
			return out;
		}
		return false;
	}

	glm::vec2 GLRenderPass::getVector2(const std::string &name)
	{
		glm::vec2 out;
		if (getValue<glm::vec2>(name, GLUniformType::Vec2, out))
		{
			return out;
		}
		return glm::vec2(0.f);
	}

	glm::vec3 GLRenderPass::getVector3(const std::string &name)
	{
		glm::vec3 out;
		if (getValue<glm::vec3>(name, GLUniformType::Vec3, out))
		{
			return out;
		}
		return glm::vec3(0.f);
	}

	glm::vec4 GLRenderPass::getVector4(const std::string &name)
	{
		glm::vec4 out;
		if (getValue<glm::vec4>(name, GLUniformType::Vec4, out))
		{
			return out;
		}
		return glm::vec4(0.f);
	}

	glm::mat4 GLRenderPass::getMatrix4(const std::string &name)
	{
		glm::mat4 out;
		if (getValue<glm::mat4>(name, GLUniformType::Mat4, out))
		{
			return out;
		}
		return glm::mat4(1.f);
	}
}