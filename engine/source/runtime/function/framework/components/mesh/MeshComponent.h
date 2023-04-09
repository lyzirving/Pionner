#ifndef __MESH_COMPONENT_H__
#define __MESH_COMPONENT_H__

#include <vector>
#include "function/framework/components/Component.h"

namespace Pionner
{
	struct GameObjPartDesc;

	class MeshComponent : public Component
	{
	public:
		MeshComponent();
		virtual ~MeshComponent();

		virtual void postLoadResource(const std::weak_ptr<GameObject>& parent) override;
		virtual void tick(float delta) override;

	private:
		using MeshHolder = std::vector<GameObjPartDesc>;
		MeshHolder m_meshes[2];
		int        m_curIndex;
		int        m_nextIndex;
		bool       m_dirty;
	};
}

#endif