#include "LightTechBase.h"
#include "CascadeShadowMap.h"

#include "asset/AssetMgr.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "LightTechBase"

namespace pio
{
	LightTechBase::LightTechBase() : Asset()
	{
	}

	LightTechBase::~LightTechBase() = default;

	Ref<LightTechBase> LightTechBase::Create(LightTech type)
	{
		switch (type)
		{
			case LightTech::CascadeShadowMap:
				return AssetMgr::MakeRuntimeAsset<CascadeShadowMap>();
			default:
				LOGE("err! light tech[%u] hasn't been implemented", type);
				std::abort();
				return Ref<LightTechBase>();
		}
	}

	template<>
	bool Asset::is<LightTechBase>() const { return assetType() == AssetType::LightTech; }
}