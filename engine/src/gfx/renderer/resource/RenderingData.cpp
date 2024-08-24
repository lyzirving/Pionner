#include "RenderingData.h"

#include "scene/Entity.h"

namespace pio
{
	RenderingEntities::RenderingEntities(const RenderingEntities &rhs)
	{
		MainLight = rhs.MainLight;
		Mesh = rhs.Mesh;
		SkinnedMesh = rhs.SkinnedMesh;
	}

	RenderingEntities::RenderingEntities(RenderingEntities &&rhs) noexcept
	{
		MainLight = std::move(rhs.MainLight);
		Mesh = std::move(rhs.Mesh);
		SkinnedMesh = std::move(rhs.SkinnedMesh);
	}

	RenderingEntities &RenderingEntities::operator=(const RenderingEntities &rhs)
	{
		if(this != &rhs)
		{
			this->RenderingEntities::RenderingEntities(rhs);
		}
		return *this;
	}

	RenderingEntities &RenderingEntities::operator=(RenderingEntities &&rhs) noexcept
	{
		if(this != &rhs)
		{
			this->RenderingEntities::RenderingEntities(std::forward<RenderingEntities>(rhs));
		}
		return *this;
	}

	RenderingData::RenderingData(const RenderingData &rhs)
	{
		UnimBuffSet = rhs.UnimBuffSet;
	}

	RenderingData::RenderingData(RenderingData &&rhs) noexcept
	{
		UnimBuffSet = std::move(rhs.UnimBuffSet);
	}

	RenderingData &RenderingData::operator=(const RenderingData &rhs)
	{
		if(this != &rhs)
		{
			this->RenderingData::RenderingData(rhs);
		}
		return *this;
	}

	RenderingData &RenderingData::operator=(RenderingData &&rhs) noexcept
	{
		if(this != &rhs)
		{
			this->RenderingData::RenderingData(std::forward<RenderingData>(rhs));
		}
		return *this;
	}
}