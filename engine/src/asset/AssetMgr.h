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

		template<typename T, typename ... Args>
		static Ref<T> MakeNamedAsset(const std::string& key, Args&& ... args)
		{
			if(!std::is_base_of<Asset, T>())
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
			std::lock_guard<std::mutex> lk{ k_NamedMutex };
			k_NamedAssets[key] = asset;
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

		template<typename T>
		static Ref<T> GetNamedAsset(const std::string& key)
		{
			if(!std::is_base_of<Asset, T>())
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
			std::lock_guard<std::mutex> lk{ k_NamedMutex };
			auto it = k_NamedAssets.find(key);
			if(it != k_NamedAssets.end())
				asset = RefCast<Asset, T>(it->second);
			return asset;
		}

	private:
		AssetMgr() {}
		~AssetMgr() = default;

	private:
		static std::unordered_map<uint32_t, Ref<Asset>> k_RuntimeAssets;
		static std::unordered_map<std::string, Ref<Asset>> k_NamedAssets;
		static std::mutex k_RuntimeMutex, k_NamedMutex;
	};
}

#endif