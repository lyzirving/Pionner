#include "Asset.h"

namespace pio
{
	Asset::Asset() : m_hnd(), m_parentHnd(0)
	{
	}

	Asset::Asset(const Asset& rhs) : m_hnd(rhs.m_hnd), m_parentHnd(rhs.m_parentHnd)
	{
	}

	Asset::Asset(Asset&& rhs) noexcept
	{
		m_hnd = rhs.m_hnd;
		m_parentHnd = rhs.m_parentHnd;

		rhs.m_hnd = rhs.m_parentHnd = InvalidId;
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
}