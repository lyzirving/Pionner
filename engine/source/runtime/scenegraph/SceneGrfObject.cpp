#include "SceneGrfObject.h"

namespace pio
{
	namespace sgf
	{
		uint32_t SceneGrfObject::g_sceneGrfId{ 0 };

		SceneGrfObject::SceneGrfObject() : m_id(g_sceneGrfId++)
		{
			m_name = "SceneGraph Object[" + std::to_string(m_id) + "]";
		}

		SceneGrfObject::SceneGrfObject(const std::string &name) : m_id(g_sceneGrfId++), m_name(name)
		{
		}

		SceneGrfObject::~SceneGrfObject() = default;
	}
}