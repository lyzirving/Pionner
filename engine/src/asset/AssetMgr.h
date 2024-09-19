#ifndef __PIONNER_ASSET_ASSET_MGR_H__
#define __PIONNER_ASSET_ASSET_MGR_H__

#include "Asset.h"

namespace pio
{
	class AssetMgr
	{
	public:
		static void Init();
		static void Shutdown();

		template<typename T, typename ... Args>
		static Ref<T> MakeRuntimeAsset(Args&& ... args)
		{
			if (!std::is_base_of<Asset, T>())
			{
				#ifdef LOCAL_TAG
				#undef LOCAL_TAG
				#endif
				#define LOCAL_TAG "AssetMgr"
				const std::type_info& classInfo = typeid(T);
				LOGE("invalid derived class[%s]", classInfo.name());
				return Ref<T>();
			}

			Ref<T> asset = CreateRef<T>(std::forward<Args>(args)...);
			std::lock_guard<std::mutex> lk{ k_RuntimeMutex };
			k_RuntimeAssets[asset->assetHnd()] = asset;
			return asset;
		}

		template<typename T>
		static Ref<T> GetRuntimeAsset(const UUID32& id)
		{
			if (!std::is_base_of<Asset, T>())
			{
				#ifdef LOCAL_TAG
				#undef LOCAL_TAG
				#endif
				#define LOCAL_TAG "AssetMgr"
				const std::type_info& classInfo = typeid(T);
				LOGE("invalid derived class[%s]", classInfo.name());
				return Ref<T>();
			}

			Ref<T> asset{ nullptr };
			std::lock_guard<std::mutex> lk{ k_RuntimeMutex };
			auto it = k_RuntimeAssets.find(id);
			if (it != k_RuntimeAssets.end())
				asset = RefCast<Asset, T>(it->second);
			return asset;
		}

		static void SaveRuntimeAsset(Ref<Asset> asset)
		{
			std::lock_guard<std::mutex> lk{ k_RuntimeMutex };
			k_RuntimeAssets[asset->assetHnd()] = asset;
		}

		static std::string SpritePath(const std::string& name, ImageType type);

	private:
		AssetMgr() {}
		~AssetMgr() = default;

	private:
		static std::unordered_map<uint32_t, Ref<Asset>> k_RuntimeAssets;		
		static std::mutex k_RuntimeMutex;
	};
}

#endif