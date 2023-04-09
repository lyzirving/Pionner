#include "function/framework/components/mesh/MeshComponent.h"
#include "function/framework/object/GameObjectDef.h"

#include "function/global/GlobalContext.h"
#include "function/framework/assets/AssetsSystem.h"
#include "function/framework/components/mesh/LoadMeshJob.h"

namespace Pionner
{
    MeshComponent::MeshComponent() : Component()
                                   , m_meshes()
                                   , m_curIndex(0), m_nextIndex(1)
                                   , m_dirty(false)
    {
    }

    MeshComponent::~MeshComponent()
    {
        MeshHolder().swap(m_meshes[m_curIndex]);
        MeshHolder().swap(m_meshes[m_nextIndex]);
    }

    void MeshComponent::postLoadResource(const std::weak_ptr<GameObject>& parent)
    {
        m_parent = parent;
        m_meshes[m_curIndex].clear();
        m_meshes[m_nextIndex].clear();
    }

    void MeshComponent::tick(float delta)
    {
        std::swap(m_curIndex, m_nextIndex);
        if (m_dirty)
        {
            m_dirty = false;
            
            std::shared_ptr<Job> job = std::shared_ptr<Job>(new LoadMeshJob(JOB_LOAD_ASSETS, this));
            LoadMeshJob* loadMeshJob = (LoadMeshJob *)job.get();
            loadMeshJob->m_meshToLoad.assign(m_meshes[m_curIndex].begin(), m_meshes[m_curIndex].end());
            g_runtimeCtx.m_assetsSystem->addJob(job);
        }
    }

    void MeshComponent::onJobEnd(JobResult& result)
    {

    }
}