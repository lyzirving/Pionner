#include "RenderingData.h"

#include "scene/node/Node.h"

namespace pio
{
	RenderingNodes::RenderingNodes(const RenderingNodes &rhs)
	{
		MainLight = rhs.MainLight;
		Mesh = rhs.Mesh;
		SkinnedMesh = rhs.SkinnedMesh;
	}

	RenderingNodes::RenderingNodes(RenderingNodes &&rhs) noexcept
	{
		MainLight = std::move(rhs.MainLight);
		Mesh = std::move(rhs.Mesh);
		SkinnedMesh = std::move(rhs.SkinnedMesh);
	}

	RenderingNodes &RenderingNodes::operator=(const RenderingNodes &rhs)
	{
		if(this != &rhs)
		{
			this->RenderingNodes::RenderingNodes(rhs);
		}
		return *this;
	}

	RenderingNodes &RenderingNodes::operator=(RenderingNodes &&rhs) noexcept
	{
		if(this != &rhs)
		{
			this->RenderingNodes::RenderingNodes(std::forward<RenderingNodes>(rhs));
		}
		return *this;
	}

	RenderingData::RenderingData(const RenderingData &rhs)
	{
		UnimBuffSet = rhs.UnimBuffSet;
		OpaqueMeshItems = rhs.OpaqueMeshItems;
		TransparentMeshItems = rhs.TransparentMeshItems;
	}

	RenderingData::RenderingData(RenderingData &&rhs) noexcept
	{
		UnimBuffSet = std::move(rhs.UnimBuffSet);
		OpaqueMeshItems = std::move(rhs.OpaqueMeshItems);
		TransparentMeshItems = std::move(rhs.TransparentMeshItems);
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