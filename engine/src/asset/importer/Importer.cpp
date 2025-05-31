#include "Importer.h"
#include "MeshImporter.h"
#include "MaterialImporter.h"
#include "ImageImporter.h"

namespace pio
{
	Importer::Importer(const ImportParams& params) : m_ImportParams(params)
	{
		m_FullPath = std::string(params.Path)
			.append(Path::FILE_SEPARATOR)
			.append(params.Name)
			.append(".")
			.append(params.Suffix);
	}

	Ref<Importer> Importer::Build(const ImportParams& param)
	{
		switch(param.Format)
		{
			case AssetFormat_Mesh:
				return CreateRef<MeshImporter>(param);
			case AssetFormat_Material:
				return CreateRef<MaterialImporter>(param);
			case AssetFormat_Image:
				return CreateRef<ImageImporter>(param);
			default:
				LOGE("invalid format[%u] to import", param.Format);
				std::abort();
				break;
		}
		return Ref<Importer>();
	}
}