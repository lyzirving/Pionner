#include "GLRenderAPI2.h"

namespace pio
{
	GLRenderAPI2::GLRenderAPI2(CRenderApiType type) : CRenderAPI(), m_type(type)
	{
	}

	bool GLRenderAPI2::init()
	{
		return false;
	}

	void GLRenderAPI2::shutdown()
	{
	}
}