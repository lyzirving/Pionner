#include "MeshData.h"

namespace pio
{
	MeshData::MeshData(const MeshData& rhs)
	{
		if (this != &rhs)
		{
			Vao = rhs.Vao;
			Vbo = rhs.Vbo;
			Ebo = rhs.Ebo;
		}
	}

	MeshData::MeshData(MeshData&& rhs) noexcept
	{		
		if (this != &rhs)
		{
			Vao = std::move(rhs.Vao);
			Vbo = std::move(rhs.Vbo);
			Ebo = std::move(rhs.Ebo);
		}
	}

	MeshData& MeshData::operator=(const MeshData& rhs)
	{
		if (this != &rhs)
		{
			this->MeshData::MeshData(rhs);
		}
		return *this;
	}

	MeshData& MeshData::operator=(MeshData&& rhs) noexcept
	{
		if (this != &rhs)
		{
			this->MeshData::MeshData(std::forward<MeshData>(rhs));
		}
		return *this;
	}
}