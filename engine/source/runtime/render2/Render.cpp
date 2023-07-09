#include "Render.h"

#include "scenegraph/view/layer/Layer.h"

namespace pio
{
	namespace render
	{
		Render::Render(const std::string &name) : m_name(name)
		{
		}

		Render::~Render() = default;

	}
}