#include "AssetMgr.h"

#include "base/Buffer.h"

#include "gfx/GfxDef.h"
#include "gfx/rhi/Texture.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "AssetMgr"

namespace pio
{
	std::unordered_map<uint32_t, Ref<Asset>> AssetMgr::k_RuntimeAssets{};
	std::unordered_map<std::string, Ref<Asset>> AssetMgr::k_NamedAssets{};
	std::mutex AssetMgr::k_RuntimeMutex{};
	std::mutex AssetMgr::k_NamedMutex{};

	void AssetMgr::Init()
	{
		TextureSpecificBuilder whiteBuilder;
		whiteBuilder.name(GpuAttr::WHITE_TEXTURE)
			.type(TextureType::TwoDimen)
			.format(TextureFormat::RGBA_32)
			.width(1).height(1)
			.texFilter(TextureFilterMin::Nearest, TextureFilterMag::Nearest);
		Buffer whiteData;	
		whiteData.allocate(255, 1 * 1 * 4);

		TextureSpecificBuilder blackBuilder;
		blackBuilder.name(GpuAttr::BLACK_TEXTURE)
			.type(TextureType::TwoDimen)
			.format(TextureFormat::RGBA_32)
			.width(1).height(1)
			.texFilter(TextureFilterMin::Nearest, TextureFilterMag::Nearest);
		Buffer blackData;
		blackData.allocate(0, 1 * 1 * 4);
	}

	void AssetMgr::Shutdown()
	{
		{
			std::lock_guard<std::mutex> lk{ k_RuntimeMutex };
			auto it = k_RuntimeAssets.begin();
			while(it != k_RuntimeAssets.end())
			{
				it->second.reset();
				it = k_RuntimeAssets.erase(it);
			}
		}

		{
			std::lock_guard<std::mutex> lk{ k_NamedMutex };
			auto it = k_NamedAssets.begin();
			while(it != k_NamedAssets.end())
			{
				it->second.reset();
				it = k_NamedAssets.erase(it);
			}
		}
	}
}