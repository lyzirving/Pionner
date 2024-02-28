#include "GLMaterial.h"

#include "gfx/rhi/Texture.h"
#include "gfx/rhi/Shader.h"

namespace pio
{
	GLUniform::GLUniform(const GLUniform &rhs)
		: m_name(rhs.m_name), m_dataType(rhs.m_dataType), m_byteSize(rhs.m_byteSize), m_offset(rhs.m_offset)
	{
	}

	GLUniform &GLUniform::operator=(const GLUniform &rhs)
	{
		if (this != &rhs)
		{
			m_name = rhs.m_name;
			m_dataType = rhs.m_dataType;
			m_byteSize = rhs.m_byteSize;
			m_offset = rhs.m_offset;
		}
		return *this;
	}

	GLMaterial::GLMaterial(const std::string &name) : Material(), m_name(name)
	{
		m_dataBuffer.allocate(0, DEFAULT_UNIFORM_BUFFER);
		setFlag(MaterialFlag::ShadowCasting, true);
		set(MaterialAttrs::MU_Alpha, 1.f);
	}

	GLMaterial::~GLMaterial()
	{
		m_dataBuffer.release();
		std::unordered_map < std::string, GLUniform>().swap(m_uniforms);
	}

	void GLMaterial::bind(const Ref<Shader> &shader)
	{
		if (!shader)
		{
			LOGE("empty shader");
			return;
		}

		if (!shader->isInit())
		{
			LOGE("invalid shader[%s]", shader->getName().c_str());
			return;
		}

		auto uniformIt = m_uniforms.begin();
		while (uniformIt != m_uniforms.end())
		{
			GLUniform &field = uniformIt->second;
			switch (field.m_dataType)
			{
				case GLUniformType::Bool:
					shader->setBool(field.m_name, m_dataBuffer.read<bool>(field.m_offset));
					break;
				case GLUniformType::Int:
					shader->setInt(field.m_name, m_dataBuffer.read<int32_t>(field.m_offset));
					break;
				case GLUniformType::UInt:
					shader->setUInt(field.m_name, m_dataBuffer.read<uint32_t>(field.m_offset));
					break;
				case GLUniformType::Float:
					shader->setFloat(field.m_name, m_dataBuffer.read<float>(field.m_offset));
					break;
				case GLUniformType::Vec2:
					shader->setVec2(field.m_name, m_dataBuffer.read<glm::vec2>(field.m_offset));
					break;
				case GLUniformType::Vec3:
					shader->setVec3(field.m_name, m_dataBuffer.read<glm::vec3>(field.m_offset));
					break;
				case GLUniformType::Vec4:
					shader->setVec4(field.m_name, m_dataBuffer.read<glm::vec4>(field.m_offset));
					break;
				case GLUniformType::IVec2:
					shader->setIVec2(field.m_name, m_dataBuffer.read<glm::ivec2>(field.m_offset));
					break;
				case GLUniformType::IVec3:
					shader->setIVec3(field.m_name, m_dataBuffer.read<glm::ivec3>(field.m_offset));
					break;
				case GLUniformType::IVec4:
					shader->setIVec4(field.m_name, m_dataBuffer.read<glm::ivec4>(field.m_offset));
					break;
				case GLUniformType::Mat3:
					shader->setMat3(field.m_name, m_dataBuffer.read<glm::mat3>(field.m_offset));
					break;
				case GLUniformType::Mat4:
					shader->setMat4(field.m_name, m_dataBuffer.read<glm::mat4>(field.m_offset));
					break;
				default:
					break;
			}
			uniformIt++;
		}

		auto textureIt = m_textures.begin();
		while (textureIt != m_textures.end())
		{
			TextureDescriptor &des = textureIt->second;
			des.Texture->active(des.Binding);
			des.Texture->bind();
			shader->setTextureSampler(textureIt->first, static_cast<TextureSampler>(des.Binding));
			textureIt++;
		}
	}

	void GLMaterial::unbind()
	{
		auto it = m_textures.begin();
		while (it != m_textures.end())
		{
			it->second.Texture->unbind();
			it++;
		}
	}

	void GLMaterial::set(const std::string &name, float value)
	{
		set<float>(name, GLUniformType::Float, value);
	}

	void GLMaterial::set(const std::string &name, int32_t value)
	{
		set<int32_t>(name, GLUniformType::Int, value);
	}

	void GLMaterial::set(const std::string &name, uint32_t value)
	{
		set<uint32_t>(name, GLUniformType::UInt, value);
	}

	void GLMaterial::set(const std::string &name, bool value)
	{
		set<bool>(name, GLUniformType::Bool, value);
	}

	void GLMaterial::set(const std::string &name, const glm::vec2 &value)
	{
		set<glm::vec2>(name, GLUniformType::Vec2, value);
	}

	void GLMaterial::set(const std::string &name, const glm::vec3 &value)
	{
		set<glm::vec3>(name, GLUniformType::Vec3, value);
	}

	void GLMaterial::set(const std::string &name, const glm::vec4 &value)
	{
		set<glm::vec4>(name, GLUniformType::Vec4, value);
	}

	void GLMaterial::set(const std::string &name, const glm::ivec2 &value)
	{
		set<glm::ivec2>(name, GLUniformType::IVec2, value);
	}

	void GLMaterial::set(const std::string &name, const glm::ivec3 &value)
	{
		set<glm::ivec3>(name, GLUniformType::IVec3, value);
	}

	void GLMaterial::set(const std::string &name, const glm::ivec4 &value)
	{
		set<glm::ivec4>(name, GLUniformType::IVec4, value);
	}

	void GLMaterial::set(const std::string &name, const glm::mat3 &value)
	{
		set<glm::mat3>(name, GLUniformType::Mat3, value);
	}

	void GLMaterial::set(const std::string &name, const glm::mat4 &value)
	{
		set<glm::mat4>(name, GLUniformType::Mat4, value);
	}

	void GLMaterial::set(const std::string &name, const Ref<Texture2D> &texture)
	{
		if (!texture)
			return;

		auto it = m_textures.find(name);
		if (it == m_textures.end())
		{
			TextureDescriptor ds{};
			ds.Binding = m_textures.size();
			ds.Texture = texture;
			m_textures[name] = ds;
		}
		else
		{
			it->second.Texture = texture;
		}
	}

	float GLMaterial::getFloat(const std::string &name)
	{
		float out;
		if (getValue<float>(name, GLUniformType::Float, out))
		{
			return out;
		}
		return 0.f;
	}

	int32_t GLMaterial::getInt(const std::string &name)
	{
		int32_t out;
		if (getValue<int32_t>(name, GLUniformType::Int, out))
		{
			return out;
		}
		return 0;
	}

	uint32_t GLMaterial::getUInt(const std::string &name)
	{
		uint32_t out;
		if (getValue<uint32_t>(name, GLUniformType::UInt, out))
		{
			return out;
		}
		return 0;
	}

	bool GLMaterial::getBool(const std::string &name)
	{
		bool out;
		if (getValue<bool>(name, GLUniformType::Bool, out))
		{
			return out;
		}
		return false;
	}

	glm::vec2 GLMaterial::getVector2(const std::string &name)
	{
		glm::vec2 out;
		if (getValue<glm::vec2>(name, GLUniformType::Vec2, out))
		{
			return out;
		}
		return glm::vec2(0.f);
	}

	glm::vec3 GLMaterial::getVector3(const std::string &name)
	{
		glm::vec3 out;
		if (getValue<glm::vec3>(name, GLUniformType::Vec3, out))
		{
			return out;
		}
		return glm::vec3(0.f);
	}

	glm::vec4 GLMaterial::getVector4(const std::string &name)
	{
		glm::vec4 out;
		if (getValue<glm::vec4>(name, GLUniformType::Vec4, out))
		{
			return out;
		}
		return glm::vec4(0.f);
	}

	glm::mat4 GLMaterial::getMatrix4(const std::string &name)
	{
		glm::mat4 out;
		if (getValue<glm::mat4>(name, GLUniformType::Mat4, out))
		{
			return out;
		}
		return glm::mat4(1.f);
	}

	Ref<Texture2D> GLMaterial::getTexture2D(const std::string &name)
	{
		Ref<Texture2D> ret;
		auto it = m_textures.find(name);
		if (it != m_textures.end())
		{
			if (it->second.Texture->getAssetType() == AssetType::Texture2D)
			{
				ret = std::dynamic_pointer_cast<Texture2D>(it->second.Texture);
			}
		}
		return ret;
	}

	Ref<Material> GLMaterial::copy()
	{
		Ref<Material> rhs = CreateRef<Material, GLMaterial>(m_name);
		Ref<GLMaterial> mat = RefCast<Material, GLMaterial>(rhs);
		mat->m_uniforms = m_uniforms;
		mat->m_textures = m_textures;// texture is shallow copy
		mat->m_materialFlag = m_materialFlag;
		mat->m_dataBuffer = m_dataBuffer;
		return rhs;
	}
}