#pragma once

#include "gfx/pass/RenderPass.h"

namespace pio
{
	class ImageHDR;
	class CubeMap;
	class StaticMeshComponent;

	class CubeMapBuildPass : public RenderPass
	{
		RTTR_ENABLE(RenderPass)
	public:
		CubeMapBuildPass(const Ref<ImageHDR>& image, const std::string& name);
		~CubeMapBuildPass() = default;

		Ref<ImageHDR> GetImageHDR() const { return m_ImageHDR.lock(); }		
		const Ref<StaticMeshComponent>& GetMeshComp() const { return m_MeshComp; }

		Ref<CubeMap> GetCubeMap() const;

		virtual void OnAttach(const Ref<RenderContext>& context) override;
		virtual void OnDetach(const Ref<RenderContext>& context) override;
		virtual void OnExecute(const Ref<RenderContext>& context, const Ref<CameraNode>& camera) override;

	private:
		WeakRef<ImageHDR> m_ImageHDR;
		Ref<StaticMeshComponent> m_MeshComp;				
	};
}