#ifndef __PIONNER_GFX_RESOURCES_MESH_DATA_H__
#define __PIONNER_GFX_RESOURCES_MESH_DATA_H__

#include "Common.h"

namespace pio
{
	template<typename T>
	struct Triangle
	{
		glm::vec3 Normal{ 0.f };
		T Indices[3]{ 0, 0, 0 };

		Triangle() {}
		Triangle(const glm::vec3& normal, const std::initializer_list<T>& indice) : Normal(normal)
		{
			size_t num = std::min(3, indice.size())
			auto it = indice.begin();
			for (size_t i = 0; i < num; i++) { Indices[i] = *(it + i); }
		}		
	};

	template<typename T, typename U>
	class MeshData
	{
	public:
		MeshData() {}
		~MeshData() = default;

		MeshData(const MeshData<T, U>& data)
		{
			m_vertice = data.m_vertice;
			m_indice = data.m_indice;
		}

		MeshData(MeshData<T, U>&& data) noexcept
		{
			m_vertice = std::move(data.m_vertice);
			m_indice = std::move(data.m_indice);
		}

		MeshData& operator=(const MeshData<T, U>& data)
		{
			if (this != &data)
			{
				this->MeshData<T, U>::MeshData(data);
			}
			return *this;
		}

		MeshData& operator=(MeshData<T, U>&& data) noexcept
		{
			if (this != &data)
			{
				this->MeshData<T, U>::MeshData(std::forward<MeshData<T, U>>(data));
			}
			return *this;
		}

		void* data() const { return m_vertice.empty() ? nullptr : m_vertice.data(); }
		uint32_t size() const { return m_vertice.size() * sizeof(T); }

		void* idxData() const { return m_indice.empty() ? nullptr : m_indice.data(); }
		uint32_t idxSize() const { return m_indice.size() * sizeof(U); }
		uint32_t idxCount() const { return m_indice.size(); }

		void setVertice(const std::vector<T>& vertice) { m_vertice = vertice; }
		void setVertice(std::vector<T>&& vertice) { m_vertice = std::forward<std::vector<T>>(vertice); }
		void setIndice(const std::vector<U>& indice) { m_indice = indice; }
		void setIndice(std::vector<U>&& indice) { m_indice = std::forward<std::vector<U>>(indice); }

		std::vector<T>& getVertice() { return m_vertice; }
		std::vector<U>& getIndice() { return m_indice; }

		const std::vector<T>& getVertice() const { return m_vertice; }
		const std::vector<U>& getIndice() const { return m_indice; }

	protected:
		std::vector<T> m_vertice;
		std::vector<U> m_indice;
	};

	using TriangleMesh = MeshData<Vertex3d, uint16_t>;
}

#endif