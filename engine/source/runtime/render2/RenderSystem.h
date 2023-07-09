#ifndef __PIONNER_RENDER_RENDER_SYSTEM_H__
#define __PIONNER_RENDER_RENDER_SYSTEM_H__

#include "scenegraph/view/Scene.h"

namespace pio
{
	namespace render
	{
		class RenderSystem
		{
		public:
			RenderSystem();
			~RenderSystem();

			void tick(uint64_t deltaMs);
			void shutdown();

			inline const std::shared_ptr<sgf::Scene> &getScene() { return m_scene; }

		private:
			std::shared_ptr<sgf::Scene> m_scene{ nullptr };
		};
	}
}

#endif	