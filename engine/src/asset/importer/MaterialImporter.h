#pragma once

#include "Importer.h"

namespace pio
{
	class MaterialImporter : public Importer
	{
		RTTR_ENABLE(Importer)
	public:
		MaterialImporter(const ImportParams& params);
		virtual ~MaterialImporter() = default;

		virtual Ref<Asset> Load() override;

	private:
		static Ref<Asset> ImportDefaultMaterial(const ImportParams& params);
		static Ref<Asset> ImportIMPL_Std_Assimp(const ImportParams& params);
		static Ref<Asset> ImportIMPL_Std_Default(const ImportParams& params);
		static Ref<Asset> ImportIMPL_Textured(const ImportParams& params);
	};
}