#include "RenderingData.h"

#include "scene/node/Node.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderingData"

namespace pio
{
	RenderingNodes::RenderingNodes(const RenderingNodes &rhs)
	{
		MainLight = rhs.MainLight;
		Mesh = rhs.Mesh;
		SkinnedMesh = rhs.SkinnedMesh;
		Sprite = rhs.Sprite;
		Gizmo = rhs.Gizmo;
	}

	RenderingNodes::RenderingNodes(RenderingNodes &&rhs) noexcept
	{
		MainLight = std::move(rhs.MainLight);
		Mesh = std::move(rhs.Mesh);
		SkinnedMesh = std::move(rhs.SkinnedMesh);
		Sprite = std::move(rhs.Sprite);
		Gizmo = std::move(rhs.Gizmo);
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

	MeshRenderingItem::MeshRenderingItem(const MeshRenderingItem& rhs)
	{
		Mode = rhs.Mode;
		RenderBuffFilter = rhs.RenderBuffFilter;
		MaterialFilter = rhs.MaterialFilter;
	}

	MeshRenderingItem::MeshRenderingItem(MeshRenderingItem&& rhs) noexcept
	{
		Mode = rhs.Mode;
		RenderBuffFilter = rhs.RenderBuffFilter;
		MaterialFilter = rhs.MaterialFilter;

		rhs.Mode = RenderingMode_Num;
		rhs.RenderBuffFilter = rhs.MaterialFilter = InvalidId;
	}

	MeshRenderingItem& MeshRenderingItem::operator=(const MeshRenderingItem& rhs)
	{
		if (this != &rhs)
		{
			this->MeshRenderingItem::MeshRenderingItem(rhs);
		}
		return *this;
	}

	MeshRenderingItem& MeshRenderingItem::operator=(MeshRenderingItem&& rhs) noexcept
	{
		if (this != &rhs)
		{
			this->MeshRenderingItem::MeshRenderingItem(std::forward<MeshRenderingItem>(rhs));
		}
		return *this;
	}

	RenderingData::RenderingData(const RenderingData &rhs)
	{
		UnimBuffSet = rhs.UnimBuffSet;
		OpaqueMeshItems = rhs.OpaqueMeshItems;
		TransparentMeshItems = rhs.TransparentMeshItems;
		UiSprites = rhs.UiSprites;
		Outlines = rhs.Outlines;
	}

	RenderingData::RenderingData(RenderingData &&rhs) noexcept
	{
		UnimBuffSet = std::move(rhs.UnimBuffSet);
		OpaqueMeshItems = std::move(rhs.OpaqueMeshItems);
		TransparentMeshItems = std::move(rhs.TransparentMeshItems);
		UiSprites = std::move(rhs.UiSprites);
		Outlines = std::move(rhs.Outlines);
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

	void RenderingData::submitMesh(MeshRenderingItem&& item)
	{
		switch (item.Mode)
		{
			case RenderingMode_Opaque:
			{
				OpaqueMeshItems.push_back(std::forward<MeshRenderingItem>(item));
				break;
			}
			case RenderingMode_Transparent:
			{
				TransparentMeshItems.push_back(std::forward<MeshRenderingItem>(item));
				break;
			}
			default:
			{
				LOGW("warning! invalid rendering mode[%u] for mesh", item.Mode);
				break;
			}
		}
	}

	void RenderingData::submitSprite(MeshRenderingItem&& item)
	{
		switch (item.Mode)
		{
			case RenderingMode_Overlay:
			{
				UiSprites.push_back(std::forward<MeshRenderingItem>(item));
				break;
			}
			default:
			{
				LOGW("warning! invalid rendering mode[%u] for sprite", item.Mode);
				break;
			}
		}
	}

	void RenderingData::submitOutline(MeshRenderingItem&& item)
	{
		switch (item.Mode)
		{
			case RenderingMode_Overlay:
			{
				Outlines.push_back(std::forward<MeshRenderingItem>(item));
				break;
			}
			default:
			{
				LOGW("warning! invalid rendering mode[%u] for outline", item.Mode);
				break;
			}
		}
	}
}