#include "RenderContext.h"
#include "GlobalSettings.h"

#include "window/Window.h"
#include "asset/AssetMgr.h"
#include "scene/Factory.h"

#include "gfx/renderer/Renderer.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/UniformBuffer.h"
#include "gfx/rhi/ShaderCompiler.h"

#include "gfx/resource/Mesh.h"
#include "gfx/resource/TextureMgr.h"
#include "gfx/resource/MeshRenderBuffer.h"
#include "gfx/resource/RenderTarget.h"

#include "gfx/resource/material/Material.h"
#include "gfx/resource/material/MaterialMgr.h"

#include "scene/3d/LightTechBase.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderContext"

namespace pio
{
	RenderContext::RenderContext(RenderBackendFlags flag, Ref<Window>& window) : m_window(window)
	{
		m_api = RenderAPI::Create(flag);
		m_state = RenderState::Create(flag);
	}

	void RenderContext::renderLoop()
	{
		m_threadId = SystemUtil::GetThreadId();
		LOGD("enter render thread[%lu]", m_threadId);

		// Initialization
		m_window->init();
		m_window->makeCurrent();
		m_window->setVSync(true);		
		m_api->setupBackend();
		m_api->setupUiBackend(m_window->nativeWindow());

		initResource();

		LOGD("enter render loop");
		while (m_thread.isRunning())
		{
			m_window->pollEvents();

			onWindowSizeChange(0, 0, m_window->width(), m_window->height());

			waitAndRender();

			m_window->swapBuffer();

			m_frameNum++;
		}
		LOGD("exit render thread[%lu]", m_threadId);
		//Call before garbage queue is executed
		releaseResource();
		// Clear resources submitted at the last frame
		m_garbageQueue[submitIdx()].execute();

		m_api->shutdown();
		m_window->shutdown();
		m_thread.set(RenderThread::State::Idle);
	}

	void RenderContext::onBeginFrameRendering()
	{
	}

	void RenderContext::onEndFrameRendering()
	{
		std::swap(m_renderingNodes, RenderingNodes());
		std::swap(m_renderingData, RenderingData());

		auto& api = m_api;
		submitRC([&api]()
		{
			api->bindScreenFrameBuffer();
		});
	}

	void RenderContext::onBeginFrameBuffer(Ref<FrameBuffer>& frameBuffer,const RenderStateAttrs& attrs)
	{		
		if (m_bindFbo == frameBuffer->id() && frameBuffer->id() != 0)
		{
			m_state->applyStateChange(attrs);
			return;
		}		
		m_api->setViewport(0, 0, frameBuffer->width(), frameBuffer->height());
		frameBuffer->bind();
		m_state->setStateMachine(attrs);
		m_bindFbo = frameBuffer->id();
	}

	void RenderContext::onBeginFrameBuffer(Ref<FrameBuffer>& frameBuffer, const RenderStateAttrs& attrs, uint8_t depthSlot)
	{
		m_api->setViewport(0, 0, frameBuffer->width(), frameBuffer->height());
		frameBuffer->bindWritingDepth(depthSlot);
		m_state->setStateMachine(attrs);
		m_bindFbo = frameBuffer->id();
	}

	void RenderContext::onEndFrameBuffer(Ref<FrameBuffer>& frameBuffer)
	{
		if (frameBuffer)
			frameBuffer->unbind();
		m_bindFbo = 0;
		m_api->setViewport(m_vp.offsetX(), m_vp.offsetY(), m_vp.ratioW(), m_vp.ratioH());// restore viewport
	}

	void RenderContext::onBeginRenderTarget(Ref<RenderTarget>& target, const RenderStateAttrs& attrs)
	{
		onBeginFrameBuffer(target->frameBuffer(), attrs);
	}

	void RenderContext::onEndRenderTarget(Ref<RenderTarget>& target)
	{
		onEndFrameBuffer(target->frameBuffer());
	}

	bool RenderContext::bSameAsBoundFrameBuffer(Ref<FrameBuffer>& frameBuffer)
	{
		return m_bindFbo == frameBuffer->id();
	}

	bool RenderContext::bSameAsBoundTarget(Ref<RenderTarget>& target)
	{
		return bSameAsBoundFrameBuffer(target->frameBuffer());
	}

	bool RenderContext::bindUnimBlock(Ref<Shader>& shader, Ref<UniformBuffer>& unimBuff, const std::string& blockName)
	{
		return m_api->bindUnimBlock(shader->id(), unimBuff->binding(), blockName);
	}

	void RenderContext::drawTriangles(Ref<MeshRenderBuffer>& meshBuff)
	{
		meshBuff->Vao->bind();
		meshBuff->Ebo->bind();

		m_api->drawElements(DrawMode_Triangle, meshBuff->Ebo->indexCount(), meshBuff->Ebo->internalFmt());

		meshBuff->Ebo->unbind();
		meshBuff->Vao->unbind();
	}

	void RenderContext::drawLines(Ref<MeshRenderBuffer>& meshBuff)
	{
		meshBuff->Vao->bind();
		meshBuff->Ebo->bind();

		m_api->drawElements(DrawMode_Line, meshBuff->Ebo->indexCount(), meshBuff->Ebo->internalFmt());

		meshBuff->Ebo->unbind();
		meshBuff->Vao->unbind();
	}

	Ref<LightTechBase> RenderContext::getLightTech(LightTech type)
	{
		uint8_t idx = PIO_UINT8(type);
		if (!m_lightTech[idx])
		{
			m_lightTech[idx] = LightTechBase::Create(self(), type);
		}
		return m_lightTech[idx];
	}

	Ref<Texture> RenderContext::createTexture(const TextureSpecific& spec)
	{
		return m_textureMgr->create(spec);
	}

	Ref<Texture> RenderContext::createTexture(const TextureSpecific& spec, Buffer& buffer)
	{
		return m_textureMgr->create(spec, buffer);
	}

	Ref<Texture> RenderContext::createTexture(const TextureSpecific& spec, const std::string& path)
	{
		return m_textureMgr->create(spec, path);
	}

	Ref<Texture> RenderContext::getTexture(const std::string& name)
	{
		return m_textureMgr->get(name);
	}

	Ref<Material> RenderContext::createMaterial(const std::string& name, ShaderSpecifier spec, RenderingMode mode)
	{
		return m_materialMgr->create(name, spec, mode);
	}

	Ref<Material> RenderContext::getMaterial(const std::string& name, bool copy)
	{
		auto material = m_materialMgr->get(name);
		if (material && copy)
		{
			auto copyItem = RefCast<Asset, Material>(material->clone());
			AssetMgr::SaveRuntimeAsset(copyItem);
			return copyItem;
		}
		return material;
	}

	void RenderContext::initResource()
	{
		auto context = self();	

		for (uint8_t i = 0; i < PIO_UINT8(ShaderType::Num); i++)
		{
			m_shaders[i] = ShaderCompiler::Compile(context, ShaderUtils::GetShaderPath(ShaderType(i)));
		}
		m_textureMgr = CreateRef<TextureMgr>();
		m_materialMgr = CreateRef<MaterialMgr>();

		m_textureMgr->init(context);
		m_materialMgr->init(context);

		auto meshData = Factory::MakeScreenQuad();
		auto screenMesh = CreateRef<Mesh>();
		screenMesh->setData(meshData);

		m_screenMeshBuffer = CreateRef<MeshRenderBuffer>();
		m_screenMeshBuffer->setUp(context, meshData->getVertice(), meshData->getIndice());

		m_renderer = Renderer::Create(GlobalSettings::RenderConfig);
		m_renderer->onAttach(context);
	}

	void RenderContext::releaseResource()
	{
		auto context = self();

		m_renderer->onDetach(context);
		m_screenMeshBuffer.reset();
		m_textureMgr->release();
		m_materialMgr->release();
		for (uint8_t i = 0; i < PIO_UINT8(LightTech::Num); i++)
		{
			m_lightTech[i].reset();
		}
		for (uint8_t i = 0; i < PIO_UINT8(ShaderType::Num); i++)
		{
			m_shaders[i].reset();//Real release will be executed at garbage queue
		}		
	}

	void RenderContext::waitAndRender()
	{
		// Wait for kick, then set render thread to busy
		m_thread.waitAndSet(RenderThread::State::Kick, RenderThread::State::Busy);

		m_api->onBeginFrame(*this);

		// task before render cmd
		m_taskQueue[queueIdx()].execute();
		// rendering
		m_cmdQueue[queueIdx()].execute();
		// garbage collection
		m_garbageQueue[queueIdx()].execute();

		m_api->onEndFrame(*this);

		// finish waiting, begin execute draw cmd
		m_thread.set(RenderThread::State::Idle);
	}

	void RenderContext::onWindowSizeChange(int32_t x, int32_t y, int32_t w, int32_t h)
	{
		if(m_vp.w() != w || m_vp.h() != h || 
		   m_vp.x() != x || m_vp.y() != y)
		{
			m_vp.setX(x);
			m_vp.setY(y);
			m_vp.setW(w);
			m_vp.setH(h);
		}
	}
}