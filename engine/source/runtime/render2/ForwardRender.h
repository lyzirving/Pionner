#ifndef __PIONNER_RENDER_FORWARD_RENDER_H__
#define __PIONNER_RENDER_FORWARD_RENDER_H__

#include "Render.h"

namespace pio
{
	namespace render
	{
		class ForwardRender : public Render
		{
		public:
			ForwardRender(const std::string &name);
			virtual ~ForwardRender();

			virtual void update(const std::vector<std::shared_ptr<sgf::Layer>> &layers, sgf::RenderInfo &info) override;
			virtual void release() override;
		};
	}
}

#endif	