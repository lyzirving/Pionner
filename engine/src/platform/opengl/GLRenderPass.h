#ifndef __PIONNER_PLATFORM_OPENGL_RENDER_PASS_H__
#define __PIONNER_PLATFORM_OPENGL_RENDER_PASS_H__

#include "GLDef.h"

#include "core/Buffer.h"
#include "scene/Camera.h"

#include "gfx/rhi/RenderPass.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLRenderPass"

namespace pio
{
	class GLRenderPass : public RenderPass
	{
		PIO_UNIFORM_WRAPPER_CLASS_DECLARE(GLRenderPass)
	public:
		GLRenderPass(const RenderPassSpecification &spec);
		virtual ~GLRenderPass();

		virtual void setState(const RenderState &state) override { m_state = state; }

		const RenderPassSpecification &getSpecification() const { return m_spec; }
		virtual const RenderState &getState() const { return m_state; }
		virtual Ref<FrameBuffer> getFramebuffer() override { return m_spec.FrameBuffer; };
		virtual Ref<UniformBuffer> getUniformBuffer() override { return m_spec.DataUB; }
		virtual Camera &getCamera() override { return m_camera; }

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
		RenderPassSpecification m_spec;
		RenderState m_state;

		Buffer m_dataBuffer{};
		std::unordered_map<std::string, GLUniform> m_uniforms{};

		Camera m_camera;
	};
}

#endif