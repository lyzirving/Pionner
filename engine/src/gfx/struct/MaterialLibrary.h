#ifndef __PIONNER_GFX_STRUCT_MATERIAL_LIBRARY_H__
#define __PIONNER_GFX_STRUCT_MATERIAL_LIBRARY_H__

#include "MaterialAsset.h"

namespace pio
{
	enum MaterialType : uint8_t
	{
		MaterialType_Packed_SandyRock, MaterialType_Packed_Num,
		MaterialType_Color, MaterialType_Num
	};

	class MaterialLibrary
	{
		PIO_SINGLETON_DECLARE(MaterialLibrary)
	public:
		Ref<MaterialAsset> getMaterial(MaterialType key);
		void load();
		void release();

	private:
		static const char* ToName(MaterialType key);

	private:
		Ref<MaterialAsset> m_materialAssets[MaterialType_Num];
	};
}

#endif