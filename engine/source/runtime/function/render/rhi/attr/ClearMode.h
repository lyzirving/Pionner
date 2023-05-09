#ifndef __RHI_CLEAR_MODE_H__
#define __RHI_CLEAR_MODE_H__

#include <glm/glm.hpp>

namespace Pionner
{
	class ClearMode
	{
	public:
		ClearMode() : m_color(0.f, 0.f, 0.f, 1.f), m_clearColor(false),
			m_clearDepth(false), m_clearStencil(false)
		{
		};

		~ClearMode() = default;

	public:
		glm::vec4 m_color;
		bool m_clearColor;
		bool m_clearDepth;
		bool m_clearStencil;
	};
}

#endif