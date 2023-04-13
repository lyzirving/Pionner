#ifndef __MESH_COMPONENT_H__
#define __MESH_COMPONENT_H__

#include <vector>
#include "function/framework/components/Component.h"
#include "function/framework/job/Job.h"
#include "function/framework/event/EventDef.h"

namespace Pionner
{
	struct GameObjPartDesc;
	class  RenderEntity;

	class MeshEventArg : public EventArg
	{
	public:
		std::vector<std::shared_ptr<RenderEntity>> m_entities;
	};

	class MeshComponent : public Component, public JobObserver
	{
	public:
		MeshComponent();
		virtual ~MeshComponent();

		virtual void postLoadResource(const std::weak_ptr<GameObject>& parent) override;
		virtual void tick(float delta) override;
		virtual void tickLogicEvent(EventType type, const std::shared_ptr<EventArg>& arg) override;
		virtual void onJobEnd(JobResult& result) override;

	private:
		struct MeshCompData
		{
			std::vector<GameObjPartDesc>               m_resources;
			std::vector<std::shared_ptr<RenderEntity>> m_entities;

			void clear();
		};

		MeshCompData m_data[2];
		int          m_curIndex;
		int          m_nextIndex;
		bool         m_dirty;
	};
}

#endif