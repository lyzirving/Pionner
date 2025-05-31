#pragma once

#include "gfx/rhi/Texture.h"
#include "FrameBufferProxy.h"

namespace pio
{
	class Shader;

	class GBuffer : public FrameBufferProxy
	{
		RTTR_ENABLE(FrameBufferProxy)
	public:
		GBuffer(const Ref<RenderContext>& context, const std::string& name);
		virtual ~GBuffer() = default;

		void BindContent(const Ref<Shader>& shader);
		void UnBindContent();

		Texture2D* GetGeometryBuffer() { return m_FrameBuff->ColorBuffers()[0]->As<Texture2D>(); }
		Texture2D* GetNormalBuffer() { return m_FrameBuff->ColorBuffers()[1]->As<Texture2D>(); }
		Texture2D* GetAlbedoBuffer() { return m_FrameBuff->ColorBuffers()[2]->As<Texture2D>(); }
		Texture2D* GetMaterialBuffer() { return m_FrameBuff->ColorBuffers()[3]->As<Texture2D>(); }
		Texture2D* GetEmissionBuffer() { return m_FrameBuff->ColorBuffers()[4]->As<Texture2D>(); }
		Texture2D* GetDepthBuffer() { return m_FrameBuff->DepthBuffer()->As<Texture2D>(); }
	};
}