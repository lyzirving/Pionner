#pragma once

#include "Asset.h"

namespace pio
{
	class RenderContext;

	class LoadRet : public TaskRet
	{
		RTTR_ENABLE(TaskRet)
	public:
		LoadRet() : TaskRet() {}
		virtual ~LoadRet() = default;

	public:
		Ref<Asset> m_Asset{};
	};

	class AssetMgr
	{
		PIO_SINGLETON_DECLARE(AssetMgr)
	public:
		template<typename T = Asset>
		Ref<T> LoadAsset(const ImportParams& param)
		{
			auto t = LoadAsset_Impl(param);
			if(t)
			{
				SaveAsset(t);
				return RefCast<Asset, T>(t);
			}
			else
			{
				return Ref<T>();
			}
		}

		void LoadAssetAsync(ImportParams&& param)
		{
			TaskGraph::Get()->CreateWorkTask([this, move_param = std::move(param)]()
			{
				auto ret = CreateRef<LoadRet>();
				ret->m_Asset = LoadAsset(move_param);
				if(ret->m_Asset)
				{
					SaveAsset(ret->m_Asset);
				}
				return ret;
			});
		}

		void LoadAssetAsync(ImportParams&& param, PTaskCb&& cb)
		{
			TaskGraph::Get()->CreatePromiseTask([this, move_param = std::move(param)]()
			{
				auto ret = CreateRef<LoadRet>();
				ret->m_Asset = LoadAsset(move_param);
				if(ret->m_Asset)
				{
					SaveAsset(ret->m_Asset);
				}
				return ret;
			}, std::move(cb));
		}

		void SaveAsset(const Ref<Asset>& asset)
		{
			std::lock_guard<std::mutex> lk{ m_Mutex };
			m_Assets[asset->Key()] = asset;
		}

		void RemoveAsset(const Ref<Asset>& asset)
		{
			if(asset)
			{
				std::lock_guard<std::mutex> lk{ m_Mutex };
				m_Assets.erase(asset->Key());
			}
		}

		void RemoveAll()
		{
			std::lock_guard<std::mutex> lk{ m_Mutex };
			auto it = m_Assets.begin();
			while (it != m_Assets.end())
			{
				it = m_Assets.erase(it);
			}
		}

		template<typename T = Asset>
		Ref<T> GetAsset(const std::string& identifier)
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
			std::lock_guard<std::mutex> lk{ m_Mutex };
			auto it = m_Assets.find(identifier);
			if(it != m_Assets.end())
				asset = RefCast<Asset, T>(it->second);
			return asset;
		}

		template<typename T, typename ... Args>
		Ref<T> MakeAsset(Args&& ... args)
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
			std::lock_guard<std::mutex> lk{ m_Mutex };
			m_Assets[asset->Key()] = asset;
			return asset;
		}

	private:
		Ref<Asset> LoadAsset_Impl(const ImportParams& param);

	private:
		std::mutex m_Mutex;
		std::unordered_map<std::string, Ref<Asset>> m_Assets;
		WeakRef<RenderContext> m_Context;
	};
}