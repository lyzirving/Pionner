#include "function/framework/components/mesh/MeshComponent.h"
#include "function/framework/object/GameObjectDef.h"

#include "function/global/GlobalContext.h"
#include "function/framework/assets/AssetsSystem.h"
#include "function/framework/components/mesh/LoadMeshJob.h"

namespace Pionner
{
    MeshComponent::MeshComponent() : Component()
                                   , m_data()
                                   , m_curIndex(0), m_nextIndex(1)
                                   , m_dirty(false)
    {
    }

    MeshComponent::~MeshComponent()
    {
        m_data[m_curIndex].clear();
        m_data[m_nextIndex].clear();
    }

    void MeshComponent::postLoadResource(const std::weak_ptr<GameObject>& parent)
    {
        m_parent = parent;
        m_data[m_curIndex].clear();
        m_data[m_nextIndex].clear();
    }

    void MeshComponent::tick(float delta)
    {
        std::swap(m_curIndex, m_nextIndex);
        if (m_dirty)
        {
            m_dirty = false;

            if (!m_data[m_curIndex].m_resources.empty())
            {
                std::shared_ptr<Job> job = std::shared_ptr<Job>(new LoadMeshJob(JOB_LOAD_ASSETS, this));
                LoadMeshJob* loadMeshJob = (LoadMeshJob*)job.get();
                loadMeshJob->m_meshToLoad.assign(m_data[m_curIndex].m_resources.begin(), m_data[m_curIndex].m_resources.end());
                g_runtimeCtx.m_assetsSystem->addJob(job);
            }

            if (!m_data[m_curIndex].m_entities.empty())
            {
            }
        }
    }

    void MeshComponent::tickLogicEvent(EventType type, const std::shared_ptr<EventArg>& arg)
    {
    }

    // this method is called in data parsing thread
    void MeshComponent::onJobEnd(JobResult& result)
    {

    }

    void MeshComponent::MeshCompData::clear()
    {
        std::vector<GameObjPartDesc>().swap(m_resources);

        for (auto &item : m_entities)
        {
            item.reset();
        }
        std::vector<std::shared_ptr<RenderEntity>>().swap(m_entities);
    }
}