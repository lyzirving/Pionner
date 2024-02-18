#include <io.h>
#include <stdio.h>

#include "AssetsManager.h"
#include "AssimpMeshImporter.h"

#include "core/EventBus.h"
#include "gfx/struct/Mesh.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "AssetsManager"

namespace pio
{
	PIO_SINGLETON_IMPL(AssetsManager)

	static const std::string ASSETS_ROOT = "assets";

	AssetsManager::AssetsManager()
	{
	}

	AssetsManager::~AssetsManager() = default;

	void AssetsManager::Init()
	{
		AssetsManager::Get();
	}

	void AssetsManager::Shutdown()
	{
		std::lock_guard<std::mutex> lk{ s_mutex };
		if (s_instance)
			s_instance->clear();
		delete s_instance;
		s_instance = nullptr;
	}

	void AssetsManager::addPackedAsset(const std::string &path, const Ref<Asset> &asset)
	{
		if (path.empty() || !asset.get())
			return;

		std::lock_guard<std::mutex> lk{ m_packedMutex };
		m_packedAssets[path] = asset;
	}

	Ref<Asset> AssetsManager::getPackedAsset(const std::string &path)
	{
		Ref<Asset> ret{ nullptr };
		std::lock_guard<std::mutex> lk{ m_packedMutex };
		auto it = m_packedAssets.find(path);
		if (it != m_packedAssets.end())
			ret = it->second;
		return ret;
	}

	void AssetsManager::addRuntimeAsset(const Ref<Asset> &asset)
	{
		if (!asset)
			return;

		std::lock_guard<std::mutex> lk{ m_runtimeMutex };
		m_runtimeAssets[asset->getHandle()] = asset;
	}

	Ref<Asset> AssetsManager::getRuntimeAsset(const AssetHandle &handle)
	{
		Ref<Asset> ret{ nullptr };
		std::lock_guard<std::mutex> lk{ m_runtimeMutex };
		auto it = m_runtimeAssets.find(handle);
		if (it != m_runtimeAssets.end())
			ret = it->second;
		return ret;
	}

	void AssetsManager::clear()
	{
		std::lock_guard<std::mutex> lk{ m_packedMutex };
		auto it = m_packedAssets.begin();
		while (it != m_packedAssets.end())
		{
			it->second.reset();
			it = m_packedAssets.erase(it);
		}

		std::lock_guard<std::mutex> runtimeLk{ m_runtimeMutex };
		auto rIt = m_runtimeAssets.begin();
		while (rIt != m_runtimeAssets.end())
		{
			rIt->second.reset();
			rIt = m_runtimeAssets.erase(rIt);
		}
	}

	std::string AssetsManager::AssetsRootDirectory(AssetFmt fmt)
	{
		switch (fmt)
		{
			case AssetFmt::Obj:
			case AssetFmt::Dae:
			case AssetFmt::GLTF:
			case AssetFmt::GLB:
				return ASSETS_ROOT + "/mesh";
			case AssetFmt::PNG:
			case AssetFmt::HDR:
				return ASSETS_ROOT + "/pic";
			default:
				LOGE("invalid assets type[%u]", fmt);
				break;
		}
		return std::string();
	}

	bool AssetsManager::FindAssetAbsPath(const std::string &parentDir, AssetFmt fmt, std::string &out)
	{
		bool found{ false };
		intptr_t fileHandle{ -1 };
		// file information
		struct _finddata_t fileInfo {};

		std::string p = parentDir + "*";

		if ((fileHandle = _findfirst(p.c_str(), &fileInfo)) != -1)
		{
			do
			{
				if (fileInfo.attrib & _A_SUBDIR)
				{
					// if directory is not current path nor parent path, continue find the file
					if (std::strcmp(fileInfo.name, ".") != 0 && std::strcmp(fileInfo.name, "..") != 0)
					{
						found = FindAssetAbsPath(p.assign(parentDir).append(fileInfo.name).append("/"), fmt, out);
						if (found)
							break;
					}
				}
				else
				{
					const char *occur = std::strchr(fileInfo.name, '.');
					if (occur && std::strcmp(occur, GetFmtPostfix(fmt)) == 0)
					{
						out = parentDir + fileInfo.name;
						found = true;
						break;
					}
				}
			} while (_findnext(fileHandle, &fileInfo) == 0);// if success, _findnext() return 0.
		}

		if (fileHandle != -1)
			_findclose(fileHandle);

		return found;
	}

	const char *AssetsManager::GetFmtPostfix(AssetFmt fmt)
	{
		switch (fmt)
		{
			case pio::AssetFmt::Obj:
				return ".obj";
			case pio::AssetFmt::Dae:
				return ".dae";
			case pio::AssetFmt::GLB:
				return ".glb";
			case pio::AssetFmt::GLTF:
				return ".gltf";
			case pio::AssetFmt::PNG:
				return ".png";
			case pio::AssetFmt::HDR:
				return ".hdr";
			default:
				return "invalid fmt";
		}
	}
}