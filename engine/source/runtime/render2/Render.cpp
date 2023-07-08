#include "Render.h"

namespace pio
{
	namespace render
	{
		Render::Render()
		{
		}

		Render::Render(const std::string &name) : m_name(name)
		{
		}

		Render::~Render() = default;

		void Render::tick(uint64_t deltaMs)
		{
		}

		void Render::release()
		{
			// only reset m_scene filed, scene will be released in PioWorld.
			m_scene.reset();
		}

		void Render::setScene(const std::shared_ptr<sgf::Scene> &scene)
		{
			if (m_scene) 
			{ 
				m_scene->release(); 
			}
			m_scene = scene;
		}

	}
}