#ifndef __PIONNER_RENDER_RENDER_H__
#define __PIONNER_RENDER_RENDER_H__

#include <string>
#include <memory>
#include <vector>

#include "scenegraph/SceneGrfDef.h"

namespace pio
{
	namespace sgf
	{
		class Layer;
	}

	namespace render
	{
		class Render
		{
		public:
			Render(const std::string &name);
			virtual ~Render();

			virtual void update(const std::vector<std::shared_ptr<sgf::Layer>> &layers, sgf::RenderInfo &info) = 0;
			virtual void release() = 0;

		protected:
			std::string m_name{};
		};
	}
}

#endif	