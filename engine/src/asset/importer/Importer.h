#pragma once

#include "asset/def/AssetDef.h"

namespace pio
{
	class Asset;
	
	class Importer : public Object
	{
		RTTR_ENABLE(Object)
	public:
		virtual Ref<Asset> Load() = 0;
		const std::string& FullPath() const { return m_FullPath; }

	public:
		static Ref<Importer> Build(const ImportParams& param);

	public:
		virtual ~Importer() = default;

	protected:
		Importer(const ImportParams& params);

	protected:
		std::string m_FullPath{};
		ImportParams m_ImportParams{};
	};
}