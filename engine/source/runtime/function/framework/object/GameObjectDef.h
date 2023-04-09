#ifndef __GAME_OBJECT_DEF_H__
#define __GAME_OBJECT_DEF_H__

#include <string>
#include <glm/glm.hpp>

namespace Pionner
{
	struct GameObjMeshDesc
	{
		std::string m_meshFile;
	};

	struct GameObjMaterialDesc
	{
		std::string m_baseColorTextureFile;
	};

	struct GameObjTransformDesc
	{
		glm::mat4 m_transform;
	};

	struct GameObjPartDesc
	{
		GameObjMeshDesc      m_meshDesc;
		GameObjMaterialDesc  m_materialDesc;
		GameObjTransformDesc m_transformDesc;
	};
}

#endif