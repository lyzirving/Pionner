#ifndef __PIONNER_GFX_BUFFER_INDICE_BUFFER_H__
#define __PIONNER_GFX_BUFFER_INDICE_BUFFER_H__

#include "Buffer.h"

namespace pio
{
	namespace gfx
	{
		class IndiceBuffer : public Buffer
		{
		public:
			IndiceBuffer();
			virtual ~IndiceBuffer();

			virtual void upload() override;

			virtual void bind() override;
			virtual void unbind() override;

			virtual uint32_t size() override;
			virtual void     release() override;

			void setIndiceArray(const std::vector<uint32_t> &indiceArray);

		private:
			std::vector<uint32_t> m_indiceArray{};
		};

		template<>
		bool Buffer::is<IndiceBuffer>();

		template<>
		IndiceBuffer *Buffer::as<IndiceBuffer>();
	}
}

#endif