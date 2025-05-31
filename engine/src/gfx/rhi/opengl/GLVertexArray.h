#pragma once

#include "gfx/rhi/VertexArray.h"

namespace pio
{
	class GLVertexArray : public VertexArray
	{
		RTTR_ENABLE(VertexArray)
	public:
		GLVertexArray(const Ref<RenderContext>& context);
		virtual ~GLVertexArray() = default;

		virtual bool Init() override;
		virtual void Destroy() override;
		virtual bool IsInit() const override { return m_ID != 0; }

		virtual void Bind() override;
		virtual void UnBind() override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) override;
		virtual const std::vector<Ref<VertexBuffer>> &GetVertexBuffers() const override { return m_VertexBuffers; }
		

	private:
		void BindVertexBuffers();
		void BindBufferLayout(const VertexBufferLayout &layout, uint32_t &slot);

	private:
		uint32_t m_BindIndex{ 0 };
		std::vector<Ref<VertexBuffer>> m_VertexBuffers{};
	};
}