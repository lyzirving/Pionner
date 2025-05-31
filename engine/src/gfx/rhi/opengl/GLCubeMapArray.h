#pragma once

#include "gfx/rhi/Texture.h"

namespace pio
{
	class GLCubeMapArray : public CubeMapArray
	{
		RTTR_ENABLE(CubeMapArray)
	public:
		GLCubeMapArray(const Ref<RenderContext>& context, const TextureSpecific& spec);
		~GLCubeMapArray() = default;

		virtual bool Init() override;
		virtual void Destroy() override;
		virtual bool IsInit() const { return m_ID != 0; }

		virtual void Bind() override;
		virtual bool BindAt(const Ref<Shader>& shader, const std::string& name) override;
		virtual void UnBind() override;

		virtual uint32_t Size() const override { return m_Size; }

		virtual void Active(TextureSampler sampler) override;
		virtual void* Data() const override { return nullptr; }
	};
}