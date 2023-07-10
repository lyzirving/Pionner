#include "ForwardRender.h"

#include "scenegraph/view/layer/Layer.h"

namespace pio
{
	namespace render
	{
		ForwardRender::ForwardRender(const std::string &name) : Render("forward_render: " + name)
		{
		}

		ForwardRender::~ForwardRender()
		{
		}

		void ForwardRender::update(const std::vector<std::shared_ptr<sgf::Layer>> &layers, sgf::RenderInfo &info)
		{
			for (auto &layer : layers)
			{
				if (layer)
				{
					layer->update(info);
				}
			}
		}

		void ForwardRender::release()
		{
		}
	}
}