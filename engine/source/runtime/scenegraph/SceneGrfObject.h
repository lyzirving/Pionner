#ifndef __SCENE_GRAPH_OBJECT_H__
#define __SCENE_GRAPH_OBJECT_H__

#include <string>

namespace pio
{
	namespace scenegrf
	{
		class SceneGrfObject
		{
		public:
			SceneGrfObject();
			SceneGrfObject(const std::string &name);
			virtual ~SceneGrfObject();

			inline const std::string &getName() { return m_name; }
			inline const uint32_t     getId() { return m_id; }

			inline void setName(const std::string &name) { m_name = name; }

		protected:
			static uint32_t g_sceneGrfId;

		protected:
			uint32_t    m_id;
			std::string m_name;
		};
	}
}

#endif