#include "MeshComp.h"

#include "global/GlobalContext.h"

#include "render/RenderSystem.h"

#include "render/resource/RenderResourceMgr.h"
#include "render/resource/buffer/GfxBuffer.h"

namespace pio
{
	pio::MeshComp::MeshComp()
		: Comp()
		, m_mat(1.f)
		, m_color(1.f)
		, m_vBufSlot(-1), m_indBufSlot(-1)
		, m_initialized(false)
	{
	}

	MeshComp::~MeshComp()
	{
		//if (m_initialized)
		//{
		//	std::shared_ptr<RenderResourceMgr> resourceMgr{ nullptr };
		//	if (g_runtimeCtx.m_renderSystem && (resourceMgr = g_runtimeCtx.m_renderSystem->getResourceMgr()))
		//	{	// if render system or resource mgr is null, all the resources should have already been released 
		//		// by call checkAbandoned() or clearActive().
		//		if (m_vBufSlot >= 0) resourceMgr->notifyRelease(BUF_VERTEX, m_vBufSlot);
		//		if (m_indBufSlot >= 0) resourceMgr->notifyRelease(BUF_INDICE, m_indBufSlot);
		//	}
		//	m_vBufSlot = -1;
		//	m_indBufSlot = -1;
		//	m_initialized = false;
		//}
	}

	void MeshComp::initialize(std::vector<Vertex> &vertexArray, std::vector<uint32_t> &indiceArray)
	{
		/*if (!m_initialized && !vertexArray.empty() && !indiceArray.empty())
		{
			std::shared_ptr<GfxBuffer> vBuffer{ nullptr }, indBuffer{ nullptr };

			auto resourceMgr = g_runtimeCtx.m_renderSystem->getResourceMgr();

			vBuffer = resourceMgr->allocate(BUF_VERTEX);
			indBuffer = resourceMgr->allocate(BUF_INDICE);

			vBuffer->insertData<Vertex>(vertexArray);
			indBuffer->insertData<uint32_t>(indiceArray);

			vBuffer->upload();
			indBuffer->upload();

			m_vBufSlot = vBuffer->getSlot();
			m_indBufSlot = indBuffer->getSlot();

			m_initialized = true;
		}*/
	}
}