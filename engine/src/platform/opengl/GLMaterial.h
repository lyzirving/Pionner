#ifndef __PIONNER_PLATFORM_OPENGL_GL_MATERIAL_H__
#define __PIONNER_PLATFORM_OPENGL_GL_MATERIAL_H__

#include "GLDef.h"

#include "core/Buffer.h"
#include "gfx/rhi/Material.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLMaterial"

namespace pio
{
	class GLMaterial : public Material
	{
		PIO_UNIFORM_WRAPPER_CLASS_DECLARE(GLMaterial)
	public:
		GLMaterial(const std::string &name);
		virtual ~GLMaterial();

		virtual void bind(const Ref<Shader> &shader) override;
		virtual void unbind() override;

		virtual void set(const std::string &name, const Ref<Texture2D> &texture) override;
		virtual Ref<Texture2D> getTexture2D(const std::string &name) override;

		virtual const std::string &getName() const override { return m_name; }

		virtual bool getFlag(MaterialFlag flag) const override { return m_materialFlag & (uint32_t)flag; }
		virtual void setFlag(MaterialFlag flag, bool value = true) override { if (value) { m_materialFlag |= (uint32_t)flag; } else { m_materialFlag &= ~(uint32_t)flag; } }

		virtual Ref<Material> copy() override;

	private:
		static const uint32_t DEFAULT_UNIFORM_BUFFER = 5 * 1024;

		struct TextureDescriptor
		{
			uint32_t Binding{ 0 };
			Ref<Texture> Texture;
		};

	private:
		template<typename T>
		void set(const std::string &name, GLUniformType type, const T &val)
		{
			auto it = m_uniforms.find(name);
			if (it != m_uniforms.end())
			{
				GLUniform &field = it->second;
				if (field.m_dataType == type)
					m_dataBuffer.replace(&val, field.m_byteSize, field.m_offset);
				else
					LOGE("invalid input type[%s] for filed[%s][%s]",
						 GetGLUniformTypeStr(type), field.m_name.c_str(), GetGLUniformTypeStr(field.m_dataType));
				return;
			}
			GLUniform field(name, type);
			field.m_byteSize = sizeof(T);
			field.m_offset = m_dataBuffer.getSize();
			m_dataBuffer.write(&val, field.m_byteSize);
			m_uniforms[field.m_name] = field;
		}

		template<typename T>
		bool getValue(const std::string &name, GLUniformType type, T &out)
		{
			auto it = m_uniforms.find(name);
			if (it != m_uniforms.end())
			{
				GLUniform &field = it->second;
				if (field.m_dataType != type)
				{
					LOGE("invalid input type[%s] for filed[%s][%s]",
						 GetGLUniformTypeStr(type), field.m_name.c_str(), GetGLUniformTypeStr(field.m_dataType));
					return false;
				}
				out = m_dataBuffer.read<T>(field.m_offset);
				return true;
			}
			return false;
		}

	private:
		const std::string m_name;
		Buffer m_dataBuffer{};
		std::unordered_map<std::string, GLUniform> m_uniforms{};
		std::unordered_map<std::string, TextureDescriptor> m_textures{};
		uint32_t m_materialFlag{ 0 };
	};
}

#endif