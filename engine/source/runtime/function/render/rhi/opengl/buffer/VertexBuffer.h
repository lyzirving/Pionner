#ifndef __VERTEX_BUFFER_H__
#define __VERTEX_BUFFER_H__

#include <vector>

#include "function/render/resource/buffer/GfxBuffer.h"

namespace Pionner
{
	struct GfxVertex;

	class VertexBuffer : public GfxBuffer
	{
	public:
		VertexBuffer(const std::shared_ptr<RenderResourceMgr> &mgr);
		virtual ~VertexBuffer();

		virtual void upload() override;
		virtual void release() override;

		virtual void bindToTarget(uint32_t target = 0) override;

	private:
		std::vector<GfxVertex> m_vertex;
	};
}

#endif