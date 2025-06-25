#include "IndirectLight.h"
#include "RenderContext.h"

#include "GlobalSettings.h"

#include "gfx/rhi/Texture.h"
#include "gfx/rhi/Shader.h"
#include "gfx/rhi/Texture.h"

#include "asset/AssetMgr.h"
#include "asset/image/Image.h"
#include "asset/image/ImageHDR.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "IndirectLight"

namespace pio
{
	UIndirectLightBuffer::UIndirectLightBuffer(const Ref<RenderContext>& context, const std::string& name)
		: UBufferProxy(context, name)
	{
		m_UBlock = CreateRef<UniformBlock>();
		m_UBlock->PushBack("SH", UniformBlock::CreateData(UniformType::Vec3Array, "SH", 9));
		m_UBlock->PushBack("Valid", UniformBlock::CreateData(UniformType::Bool, "Valid"));
		m_UBlock->Calculate();

		m_UBuffer = UniformBuffer::Create(m_Context.lock(), m_UBlock->GetByteUsed(),
										  UBBinging_IndirectLight, BufferUsage::Dynamic);

		//@todo: read the prebuild file
		glm::vec3 sphericalHarmonics[9] = 
		{
			glm::vec3(0.785786807537079,  0.785786807537079,  0.785786807537079), // L00, irradiance, pre-scaled base
			glm::vec3(0.402588516473770,  0.402588516473770,  0.402588516473770), // L1-1, irradiance, pre-scaled base
			glm::vec3(0.460519373416901,  0.460519373416901,  0.460519373416901), // L10, irradiance, pre-scaled base
			glm::vec3(0.084180898964405,  0.084180898964405,  0.084180898964405), // L11, irradiance, pre-scaled base
			glm::vec3(0.058341916650534,  0.058341916650534,  0.058341916650534), // L2-2, irradiance, pre-scaled base
			glm::vec3(0.204982891678810,  0.204982891678810,  0.204982891678810), // L2-1, irradiance, pre-scaled base
			glm::vec3(0.092737942934036,  0.092737942934036,  0.092737942934036), // L20, irradiance, pre-scaled base
			glm::vec3(-0.091809459030628, -0.091809459030628, -0.091809459030628),// L21, irradiance, pre-scaled base
			glm::vec3(-0.006748968269676, -0.006748968269676, -0.006748968269676) // L22, irradiance, pre-scaled base
		};

		SetValues(sphericalHarmonics);
		OnValidChange();
	}

	void UIndirectLightBuffer::SetValues(const glm::vec3(&sphericalHarmonics)[9])
	{
		auto* spArray = (*m_UBlock)["SH"]->As<UniformArrayLayout>();
		for(size_t i = 0; i < 9; ++i)
		{
			auto& layout = (*spArray)[i];
			m_UBlock->WriteAt(glm::value_ptr(sphericalHarmonics[i]), sizeof(glm::vec3), layout.GetAlignOffset());
		}
	}

	void UIndirectLightBuffer::OnValidChange()
	{
		auto validUD = (*m_UBlock)["Valid"];
		m_UBlock->WriteAt(&m_Valid, sizeof(bool), validUD->GetAlignOffset());
	}

	IndirectLight::IndirectLight(const Ref<RenderContext>& context, const std::string& name)
		: FrameBufferProxy(context, name)
	{		
		m_UBuffer = CreateRef<UIndirectLightBuffer>(context, "SphericalHarmonicsUBuffer");
		m_IblDFG = RefCast<Texture, Texture2D>(AssetMgr::Get()->GetAsset<Image>(Path::ImageKey(ImageAsset::IBL))->GetTexture());		
		m_SSR = RefCast<Texture, Texture2D>(AssetMgr::Get()->GetAsset<Image>(Path::ImageKey(ImageAsset::WHITE))->GetTexture());		
	}

	void IndirectLight::OnTick()
	{
		TryGetIblSpecular();
		CheckValid();
		m_UBuffer->As<UIndirectLightBuffer>()->Upload();
	}

	void IndirectLight::BindAt(const Ref<Shader>& shader)
	{
		if(m_IblDFG)
			m_IblDFG->BindAt(shader, GpuAttr::UNI_IBL_DFG);

		if(m_IblSpecular)
			m_IblSpecular->BindAt(shader, GpuAttr::UNI_IBL_SPECULAR);

		if(m_SSR)
			m_SSR->BindAt(shader, GpuAttr::UNI_IBL_SSR);
	}

	void IndirectLight::UnBindAt()
	{
		if(m_IblDFG)
			m_IblDFG->UnBind();

		if(m_IblSpecular)
			m_IblSpecular->UnBind();

		if(m_SSR)
			m_SSR->UnBind();	
	}

	bool IndirectLight::TryGetIblSpecular()
	{
		if(m_IblSpecular)
			return true;

		auto image = AssetMgr::Get()->GetAsset(Path::ImageKey(ImageAsset::LIGHT_ROOM));
		if(!image)
		{
			LOGW("fail to get asset[%s]", ImageAsset::LIGHT_ROOM);
			return false;
		}

		auto imageHdr = RefCast<Asset, ImageHDR>(image);
		if(!imageHdr)
		{
			LOGW("fail to get ImageHDR from asset[%s]", ImageAsset::LIGHT_ROOM);
			return false;
		}

		if(!imageHdr->IsInit())
		{
			LOGW("ImageHDR from asset[%s] hasn't been initialized", ImageAsset::LIGHT_ROOM);
			return false;
		}

		m_IblSpecular = RefCast<Texture, CubeMap>(imageHdr->GetTexture());		
		LOGD("succeed to get ibl specular from[%s]", ImageAsset::LIGHT_ROOM);
		return true;
	}

	void IndirectLight::CheckValid()
	{
		m_UBuffer->As<UIndirectLightBuffer>()->SetValid(m_IblSpecular.use_count() != 0 && m_IblDFG.use_count() != 0);
	}
}
