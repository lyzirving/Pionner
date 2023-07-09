#ifndef __PIONNER_RENDER_RENDER_H__
#define __PIONNER_RENDER_RENDER_H__

#include "scenegraph/view/Scene.h"

namespace pio
{
	namespace render
	{
		class Render
		{
		public:
			Render();
			Render(const std::string &name);
			virtual ~Render();

			virtual void tick(uint64_t deltaMs);
			virtual void release();

			inline bool emptyScene() { return m_scene == nullptr; }

			void setScene(const std::shared_ptr<sgf::Scene> &scene);

		protected:
			std::string                 m_name{};
			std::shared_ptr<sgf::Scene> m_scene{ nullptr };
		};
	}
}

#endif	