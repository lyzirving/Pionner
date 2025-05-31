#pragma once

#include "Node.h"

namespace pio
{	
	class StaticMesh;
	struct SubMesh;

	class StaticMeshNode : public Node
	{
		RTTR_ENABLE(Node)
	public:
		StaticMeshNode();
		virtual ~StaticMeshNode() = default;

		void SetMesh(const Ref<StaticMesh>& mesh);
		void SetSubMesh(int32_t index, const SubMesh& mesh);

	protected:
		virtual void OnInit() override;
		virtual void OnBeginFrameTick(const Ref<RenderContext>& context) override;
		virtual void OnTickComponents(const Ref<RenderContext>& context) override;
		virtual void OnTick(const Ref<RenderContext>& context) override;
		virtual void OnRender(const Ref<RenderContext>& context) override;

	protected:
		AABB m_BoundingBox;

	private:
		friend class StaticMeshComponent;
	};
}