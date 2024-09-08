#ifndef __PIONNER_GFX_PASS_DEFFERED_PASS_H__
#define __PIONNER_GFX_PASS_DEFFERED_PASS_H__

#include "RenderPass.h"

namespace pio
{
	class Mesh;
	class MeshRenderBuffer;

	class DefferedPass : public RenderPass
	{
		OVERRIDE_PASS_TYPE(RenderPassType::Deferred)
	public:
		DefferedPass(const std::string &name, RenderPassEvent event) : RenderPass(name, event) {}
		~DefferedPass() = default;

		virtual void onAttach(Ref<RenderContext>& context) override;
		virtual void onDetach(Ref<RenderContext>& context) override;
		virtual void onExecute(Ref<RenderContext> &context, Ref<Camera>& camera, Ref<RenderPass>& lastPass) override;
	protected:
		Ref<Mesh> m_screenMesh;
		Ref<MeshRenderBuffer> m_screenBuff;
	};

	template<>
	bool RenderPass::is<DefferedPass>() const;
}

#endif