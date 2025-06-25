#include "CubeMapBuildPass.h"

#include "GlobalSettings.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/UniformBuffer.h"
#include "gfx/rhi/Texture.h"
#include "gfx/renderer/RenderContext.h"

#include "asset/AssetMgr.h"
#include "asset/mesh/StaticMesh.h"
#include "asset/image/ImageHDR.h"

#include "component/StaticMeshComponent.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "CubeMapBuildPass"

namespace pio
{
	CubeMapBuildPass::CubeMapBuildPass(const Ref<ImageHDR>& image, const std::string& name)
		: RenderPass(name, BeforeRendering),//Pass as a tool should be executed at the every first of rendering
		m_ImageHDR(image)
	{
	}

    Ref<CubeMap> CubeMapBuildPass::GetCubeMap() const
    {
        if(!m_FrameBuff)
            return Ref<CubeMap>();

        return RefCast<Texture, CubeMap>(m_FrameBuff->ColorBuffers()[0]);
    }

    void CubeMapBuildPass::OnAttach(const Ref<RenderContext>& context)
	{
        m_MeshComp = Component::NewComponent<StaticMeshComponent>(context);
        auto cube = AssetMgr::Get()->GetAsset(Path::MeshKey(MeshAsset::CUBE))->SpawnChild();
        m_MeshComp->SetMesh(RefCast<Asset, StaticMesh>(cube));        

        m_Attrs.SetClear(Clear::Common())
            .SetCull(CullFace::Create(FaceDirection::CouterClockwise, FaceMode::FaceMode_Front))
            .SetBlend(Blend::Disable())
            .SetDepth(DepthTest(FuncAttr::Lequal, DepthTest::Mask::ReadWrite))            
            .SetStencil(StencilTest::Disable());

        auto imgHDR = m_ImageHDR.lock();
        int32_t size = std::min(imgHDR->GetWidth(), imgHDR->GetHeight());

        FrameBufferSpecific fboSpec;
        fboSpec.Name = Name();
        fboSpec.Width = size;
        fboSpec.Height = size;
        PIO_FBO_ADD_USAGE(fboSpec.Usage, FrameBufferUsage_Color);

        TextureSpecificBuilder cubeMapBuilder;
        cubeMapBuilder.Name(std::string(imgHDR->Name()).append("-CubeMap"))
            .Type(TextureType::CubeMap)
            .Format(TextureFormat::RGB_16F)
            .Width(size).Height(size)
            .TexWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
            .TexFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);

        TextureSpecificBuilder depthBuilder;
        depthBuilder.Name(std::string(imgHDR->Name()).append("-DepthBuffer"))
            .Type(TextureType::RenderBuffer)
            .Format(TextureFormat::DEPTH_24)
            .Width(size).Height(size);

        fboSpec.ColorSpec.push_back(cubeMapBuilder.Build());
        fboSpec.DepthSpec.push_back(depthBuilder.Build());

        m_FrameBuff = FrameBuffer::Create(context, fboSpec);

        context->UploadData(m_FrameBuff);
	}

	void CubeMapBuildPass::OnDetach(const Ref<RenderContext>& context)
	{
	}

	void CubeMapBuildPass::OnExecute(const Ref<RenderContext>& context, const Ref<CameraNode>& camera)
	{        
        context->SubmitRC([weakSelf = WeakRef<CubeMapBuildPass>(Self<CubeMapBuildPass>()), weakCtx = WeakRef<RenderContext>(context)]() mutable
        {
            auto self = weakSelf.lock();
            PIO_CHECK_RETURN(self, "self is invalid in CubeMapBuildPass");
            auto ctx = weakCtx.lock();
            PIO_CHECK_RETURN(self, "context is invalid in CubeMapBuildPass");
            auto shader = ctx->FindShader(SHADER_SPEC_EQUIRECMAP);
            PIO_CHECK_RETURN(shader, "shader is invalid in CubeMapBuildPass");
            auto imgHDR = self->GetImageHDR();
            Texture2D* source = imgHDR ? imgHDR->GetEquiAngular()->As<Texture2D>() : nullptr;
            PIO_CHECK_RETURN(source, "EquiAngular texture is invalid in CubeMapBuildPass");
            Ref<CubeMap> cubeMap = self->GetCubeMap();
            PIO_CHECK_RETURN(cubeMap, "CubeMap is invalid in CubeMapBuildPass");
            Ref<StaticMeshComponent> meshComp = self->GetMeshComp();
            PIO_CHECK_RETURN(meshComp, "MeshComp is invalid in CubeMapBuildPass");

            glm::mat4 capturePrj = Math::PerspectiveMat(90.f, 1.f, 0.1f, 10.f);
            glm::mat4 captureView[CubeMapDir_Num];
            captureView[CubeMapDir_PositiveX] = Math::CameraViewMat(glm::vec3(0.f), World::Right, World::Down);
            captureView[CubeMapDir_NegativeX] = Math::CameraViewMat(glm::vec3(0.f), World::Left, World::Down);
            captureView[CubeMapDir_PositiveY] = Math::CameraViewMat(glm::vec3(0.f), World::Up, World::Forward);
            captureView[CubeMapDir_NegativeY] = Math::CameraViewMat(glm::vec3(0.f), World::Down, World::Back);
            captureView[CubeMapDir_NegativeZ] = Math::CameraViewMat(glm::vec3(0.f), World::Forward, World::Down);
            captureView[CubeMapDir_PositiveZ] = Math::CameraViewMat(glm::vec3(0.f), World::Back, World::Down);      

            ctx->OnBeginFrameBuffer(self->GetFrameBuffer(), self->GetRenderState());
            shader->Bind();
            shader->SetMat4("u_PrjMat", capturePrj);
            source->BindAt(shader, "u_EquirectangularMap");

            Clear clear;
            clear.SetColorBit().SetDepthBit();
            cubeMap->Bind();
            for(size_t i = 0; i < CubeMapDir_Num; ++i)
            {
                shader->SetMat4("u_ViewMat", captureView[i]);
                cubeMap->AttachFrameBuffer(self->GetFrameBuffer(), 0, i);
                ctx->SetClear(clear);

                ctx->DrawTriangles(meshComp->GetRenderBuffer(0));
            }

            source->UnBind();
            shader->UnBind();
            ctx->OnEndFrameBuffer(self->GetFrameBuffer());

            imgHDR->SetCubeMap(cubeMap);
        });
	}
}