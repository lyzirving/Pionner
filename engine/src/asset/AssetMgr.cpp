#include "AssetMgr.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "AssetMgr"

namespace pio
{
	std::unordered_map<uint32_t, Ref<Asset>> AssetMgr::k_RuntimeAssets{};
	std::mutex AssetMgr::k_RuntimeMutex{};

	void AssetMgr::Init()
	{		
	}

	void AssetMgr::Shutdown()
	{
		std::lock_guard<std::mutex> lk{ k_RuntimeMutex };
		auto it = k_RuntimeAssets.begin();
		while(it != k_RuntimeAssets.end())
		{
			it->second.reset();
			it = k_RuntimeAssets.erase(it);
		}
	}
}