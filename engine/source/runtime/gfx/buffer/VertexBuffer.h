#ifndef __PIONNER_GFX_BUFFER_VERTEX_BUFFER_H__
#define __PIONNER_GFX_BUFFER_VERTEX_BUFFER_H__

#include "Buffer.h"

namespace pio
{
	namespace sgf
	{
		struct Vertex;
	}

	namespace gfx
	{
		class VertexBuffer : public Buffer
		{
		public:
			VertexBuffer();
			virtual ~VertexBuffer();

			virtual void upload() override;

			virtual void bind() override;
			virtual void unbind() override;

			virtual uint32_t size() override;
			virtual void     release() override;

			void setVertexArray(const std::vector<sgf::Vertex> &vertexArray);

		private:
			uint32_t m_vao{ 0 };
			std::vector<sgf::Vertex> m_vertexArray{};
		};

		template<>
		bool Buffer::is<VertexBuffer>();

		template<>
		VertexBuffer *Buffer::as<VertexBuffer>();
	}
}

#endif