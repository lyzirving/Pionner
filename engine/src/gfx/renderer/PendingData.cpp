#include "PendingData.h"

#include "scene/Entity.h"

namespace pio
{
	PendingData::PendingData(const PendingData& rhs)
	{
		MainLight = rhs.MainLight;
		Mesh = rhs.Mesh;
		SkinnedMesh = rhs.SkinnedMesh;
	}

	PendingData::PendingData(PendingData&& rhs) noexcept
	{
		MainLight = std::move(rhs.MainLight);
		Mesh = std::move(rhs.Mesh);
		SkinnedMesh = std::move(rhs.SkinnedMesh);
	}

	PendingData& PendingData::operator=(const PendingData& rhs)
	{
		if (this != &rhs)
		{
			this->PendingData::PendingData(rhs);
		}
		return *this;
	}

	PendingData& PendingData::operator=(PendingData&& rhs) noexcept
	{
		if (this != &rhs)
		{
			this->PendingData::PendingData(std::forward<PendingData>(rhs));
		}
		return *this;
	}
}