#include "MaterialLibrary.h"

#include "asset/AssetsManager.h"
#include "asset/MaterialImporter.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "MaterialLibrary"

namespace pio
{
	PIO_SINGLETON_IMPL(MaterialLibrary)

	MaterialLibrary::MaterialLibrary()
	{
	}

	MaterialLibrary::~MaterialLibrary()
	{
	}

	void MaterialLibrary::Init()
	{
		MaterialLibrary::Get()->load();
	}

	void MaterialLibrary::Shutdown()
	{
		{
			std::lock_guard<std::mutex> lk{ s_mutex };
			if (s_instance)
				s_instance->release();
			delete s_instance;
			s_instance = nullptr;
		}
	}

	Ref<MaterialAsset> MaterialLibrary::getMaterial(MaterialType key)
	{
		if (key < MaterialType_Packed_Num)
		{
			return m_materialAssets[key];
		}
		else if (key < MaterialType_Num)
		{
			return CreateRef<MaterialAsset>(m_materialAssets[key]->m_material->copy());
		}
		LOGE("invalid material type[%u]", key);
		return Ref<MaterialAsset>();
	}

	void MaterialLibrary::load()
	{
		for (uint8_t i = 0; i < MaterialType_Packed_Num; i++)
		{
			MaterialType key = (MaterialType)i;
			std::string name = ToName(key);
			std::string path = "assets/material/" + name;
			MaterialImporter importer(path, name);
			Ref<Material> mi = importer.importMaterial();
			if (mi)
			{
				LOGD("load packed material[%s]", path.c_str());
				Ref<MaterialAsset> ma = AssetsManager::CreateRuntimeAssets<MaterialAsset>(mi);
				m_materialAssets[i] = ma;
			}
			else
			{
				LOGE("fail to load pakced material[%s]", path.c_str());
			}
		}


		// color material
		{
			std::string name = ToName(MaterialType_Color);
			MaterialImporter importer(name);
			Ref<Material> mi = importer.importColorMaterial();
			if (mi)
			{
				LOGD("load color material[%s]", name.c_str());
				Ref<MaterialAsset> ma = AssetsManager::CreateRuntimeAssets<MaterialAsset>(mi);
				m_materialAssets[MaterialType_Color] = ma;
			}
			else
			{
				LOGE("fail to load color material[%s]", name.c_str());
			}
		}
	}

	void MaterialLibrary::release()
	{
		for (auto &m : m_materialAssets)
		{
			m.reset();
		}
	}

	const char* MaterialLibrary::ToName(MaterialType key)
	{
		switch (key)
		{
			case MaterialType_Packed_SandyRock:
				return "sandy_rocks";
			case MaterialType_Color:
				return "raw_color";
			default:
				return "None";
		}
	}
}