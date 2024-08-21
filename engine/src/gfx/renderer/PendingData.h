#ifndef __PIONNER_GFX_RENDERER_PENDING_DATA_H__
#define __PIONNER_GFX_RENDERER_PENDING_DATA_H__

#include "Base.h"

namespace pio
{
	class Entity;

	//Entity collection that is about to be uploaded
	struct PendingData
	{
		PendingData() {}
		~PendingData() = default;

		PendingData(const PendingData& rhs);
		PendingData(PendingData&& rhs) noexcept;

		PendingData& operator=(const PendingData& rhs);
		PendingData& operator=(PendingData&& rhs) noexcept;

		Ref<Entity> MainLight;
		std::list<Ref<Entity>> Mesh;
		std::list<Ref<Entity>> SkinnedMesh;
	};
}

#endif