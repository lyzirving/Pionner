#ifndef __PIONNER_MESH_COMP_H__
#define __PIONNER_MESH_COMP_H__

#include <cstdint>
#include <string>
#include <vector>
#include <map>

#include "function/framework/comp/Comp.h"

namespace Pionner
{
	struct RenderParam;
	struct Vertex;

	class MeshComp : public Comp
	{
	public:
		MeshComp();
		virtual ~MeshComp();

		void initialize(std::vector<Vertex> &vertexArray, std::vector<uint32_t> &indiceArray);

	public:
		std::map<std::string, CompArg> m_args;
		glm::mat4 m_mat;
		glm::vec4 m_color;
		int32_t   m_vBufSlot, m_indBufSlot;
		bool      m_initialized;
	};
}

#endif