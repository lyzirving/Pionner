#include "Asset.h"

namespace pio
{
	Asset::Asset() : m_Hnd(), m_ParentHnd(0), m_Params()
	{
	}

	Asset::Asset(const ImportParams& params) : m_Hnd(), m_ParentHnd(0), 
		m_Params(params)
	{
		m_Context = m_Params.Context;
		m_FullPath = std::string(params.Path)
			.append(Path::FILE_SEPARATOR)
			.append(params.Name)
			.append(".")
			.append(params.Suffix);

		m_Key = std::string(params.Path)
			.append(Path::FILE_SEPARATOR)
			.append(params.Name)
			.append(".")
			.append(Path::ASSET_SUFFIX);
	}

	Asset::Asset(const Asset& rhs) : m_Hnd(rhs.m_Hnd), m_ParentHnd(rhs.m_ParentHnd), 
		m_Params(rhs.m_Params), m_FullPath(rhs.m_FullPath), m_Key(rhs.m_Key)
	{
	}

	Asset::Asset(Asset&& rhs) noexcept
	{
		m_Hnd = rhs.m_Hnd;
		m_ParentHnd = rhs.m_ParentHnd;
		m_Params = rhs.m_Params;
		m_FullPath = rhs.m_FullPath;
		m_Key = rhs.m_Key;

		rhs.m_Hnd = rhs.m_ParentHnd = InvalidId;
		std::swap(rhs.m_Params, ImportParams());
		rhs.m_FullPath.clear();
		rhs.m_Key.clear();
	}

	Asset Asset::operator=(const Asset& rhs)
	{
		if (this != &rhs)
		{
			this->Asset::Asset(rhs);
		}
		return *this;
	}

	Asset Asset::operator=(Asset&& rhs) noexcept
	{
		if (this != &rhs)
		{
			this->Asset::Asset(std::forward<Asset>(rhs));
		}
		return *this;
	}

	void Asset::Init()
	{
		OnInit();
	}
}