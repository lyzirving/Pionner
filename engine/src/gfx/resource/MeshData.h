#ifndef __PIONNER_GFX_RESOURCES_MESH_DATA_H__
#define __PIONNER_GFX_RESOURCES_MESH_DATA_H__

#include "Common.h"

namespace pio
{
	class MeshData
	{
	public:
		MeshData() {}
		virtual ~MeshData() = default;
		virtual void* data() const = 0;
		virtual uint32_t size() const = 0;

		template<typename T, typename U>
		void update(const std::vector<T>& vertex, const std::vector<U>& indice) {}
	};
}

#endif