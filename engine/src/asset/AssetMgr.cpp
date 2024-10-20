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

	std::string AssetMgr::MaterialPath(const std::string& name)
	{
		std::string path = FileUtil::ASSET_ROOT;		
		path.append(FileUtil::SEPARATOR).append("material")
			.append(FileUtil::SEPARATOR).append(name);
		return path;
	}

	std::string AssetMgr::SpritePath(const std::string& name, ImageType type)
	{
		std::string path = FileUtil::ASSET_ROOT;
		path.append(FileUtil::SEPARATOR).append("pic")
			.append(FileUtil::SEPARATOR).append("sprite")
			.append(FileUtil::SEPARATOR).append(name);
		switch (type)
		{
			case ImageType::PNG:
				path += ".png";
				break;
			case ImageType::JPG:
				path += ".jpg";
				break;
			default:
				break;
		}
		return path;
	}
}