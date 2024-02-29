#ifndef __PIONNER_ASSET_ASSETS_MANAGER_H__
#define __PIONNER_ASSET_ASSETS_MANAGER_H__

#include "Asset.h"

namespace pio
{
	class AssetsManager
	{
		PIO_SINGLETON_DECLARE(AssetsManager)
	public:
		void addPackedAsset(const std::string &path, const Ref<Asset> &asset);
		void addRuntimeAsset(const Ref<Asset> &asset);
		void clear();

	public:
		template<typename T, typename ... Args>
		static Ref<Asset> CreateRuntimeAssets(Args&& ... args)
		{
			if (!std::is_base_of<Asset, T>())
				return Ref<Asset>();
			Ref<Asset> ret = CreateRef<Asset, T>(std::forward<Args>(args)...);
			AssetsManager::Get()->addRuntimeAsset(ret);
			return ret;
		}

		static Ref<Asset> GetRuntimeAsset(const AssetHandle &handle)
		{
			return AssetsManager::Get()->getRuntimeAsset(handle);
		}

		template<typename T>
		static Ref<T> GetRuntimeAsset(const AssetHandle &handle)
		{
			if (!std::is_base_of<Asset, T>())
				return Ref<T>();

			Ref<Asset> asset = AssetsManager::Get()->getRuntimeAsset(handle);
			if (asset)
			{
				return RefCast<Asset, T>(asset);
			}
			return Ref<T>();
		}

		template<typename T, typename ... Args>
		static Ref<T> GetOrCreatePackedAsset(const std::string &path, Args&& ... args)
		{
			if (!std::is_base_of<Asset, T>())
				return Ref<T>();
			Ref<T> ret;
			Ref<Asset> cache = AssetsManager::Get()->getPackedAsset(path);
			if (!cache)
			{
				ret = T::Create(path, std::forward<Args>(args)...);
				AssetsManager::Get()->addPackedAsset(path, ret);
			}
			else
			{
				ret = RefCast<Asset, T>(cache);
			}
			return ret;
		}

		template<typename T>
		static Ref<T> GetPackedAsset(const std::string &path)
		{
			if (!std::is_base_of<Asset, T>())
				return Ref<T>();
			Ref<Asset> cache = AssetsManager::Get()->getPackedAsset(path);
			if (cache)
			{
				Ref<T> ret = RefCast<Asset, T>(cache);
				return ret;
			}
			return Ref<T>();
		}
		
		static std::string AssetsRootDirectory(AssetFmt fmt);
		static bool FindAssetAbsPath(const std::string &parentDir, AssetFmt fmt, std::string &out);
		static const char *GetFmtPostfix(AssetFmt fmt);
		static std::string SpriteAbsPath(const std::string &name, AssetFmt fmt);
		static std::string IconAbsPath(const std::string &name, AssetFmt fmt);

	private:
		Ref<Asset> getPackedAsset(const std::string &path);
		Ref<Asset> getRuntimeAsset(const AssetHandle &handle);

	private:
		// packed assets are resources that exist on disk
		std::unordered_map<std::string, Ref<Asset>> m_packedAssets{};
		std::mutex m_packedMutex{};

		// runtime assets are resources that parsed at runtime.
		std::unordered_map<AssetHandle, Ref<Asset>> m_runtimeAssets{};
		std::mutex m_runtimeMutex{};
	};
}

#endif