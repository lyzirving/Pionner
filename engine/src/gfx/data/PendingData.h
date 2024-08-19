#ifndef __PIONNER_GFX_DATA_PENDING_DATA_H__
#define __PIONNER_GFX_DATA_PENDING_DATA_H__

#include "scene/Entity.h"

namespace pio
{
	struct PendingData
	{
		PendingData() {}
		~PendingData() = default;

		PendingData(const PendingData& rhs)
		{
			MainLight = rhs.MainLight;
			Mesh = rhs.Mesh;
			SkinnedMesh = rhs.SkinnedMesh;
		}

		PendingData(PendingData&& rhs) noexcept
		{
			MainLight = std::move(rhs.MainLight);
			Mesh = std::move(rhs.Mesh);
			SkinnedMesh = std::move(rhs.SkinnedMesh);
		}

		PendingData& operator=(const PendingData& rhs)
		{
			if (this != &rhs)
			{
				MainLight = rhs.MainLight;
				Mesh = rhs.Mesh;
				SkinnedMesh = rhs.SkinnedMesh;
			}
			return *this;
		}

		PendingData& operator=(PendingData&& rhs) noexcept
		{
			if (this != &rhs)
			{
				MainLight = std::move(rhs.MainLight);
				Mesh = std::move(rhs.Mesh);
				SkinnedMesh = std::move(rhs.SkinnedMesh);
			}
			return *this;
		}

		Ref<Entity> MainLight;
		std::list<Ref<Entity>> Mesh;
		std::list<Ref<Entity>> SkinnedMesh;
	};
}

#endif