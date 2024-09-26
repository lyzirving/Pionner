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

	class MeshDataBase
	{
	public:		
		virtual ~MeshDataBase() = default;
		// Vertice array data
		virtual const void* data() const = 0;
		// Vertice byte size
		virtual uint32_t size() const = 0;

		// Indice array data
		virtual const void* indice() const = 0;
		// Indice byte size
		virtual uint32_t indiceSize() const = 0;
		// Num of item in indice array
		virtual uint32_t indiceNum() const = 0;

		// Only support coverting to leaf class
		template<typename T>
		T* as() const { return (typeid(T) == typeid(this)) ? static_cast<T*>(this) : nullptr; }
	};

	template<typename T, typename U>
	class MeshData : public MeshDataBase
	{
	public:
		MeshData() {}
		~MeshData() = default;

		virtual const void* data() const override { return m_vertice.empty() ? nullptr : m_vertice.data(); }
		virtual uint32_t size() const override { return m_vertice.size() * sizeof(T); }

		virtual const void* indice() const override { return m_indice.empty() ? nullptr : m_indice.data(); }
		virtual uint32_t indiceSize() const override { return m_indice.size() * sizeof(U); }
		virtual uint32_t indiceNum() const override { return m_indice.size(); }

		void setVertice(const std::vector<T>& vertice) { m_vertice = vertice; }
		void setVertice(std::vector<T>&& vertice) { m_vertice = std::forward<MeshData<T, U>>(vertice); }

		void setIndice(const std::vector<U>& indice) { m_indice = indice; }
		void setIndice(std::vector<U>&& indice) { m_indice = std::forward<MeshData<T, U>>(indice); }

		std::vector<T>& getVertice() { return m_vertice; }
		std::vector<U>& getIndice() { return m_indice; }

		const std::vector<T>& getVertice() const { return m_vertice; }
		const std::vector<U>& getIndice() const { return m_indice; }

	protected:
		std::vector<T> m_vertice;
		std::vector<U> m_indice;
	};

	using TriangleMesh = MeshData<Vertex3d, uint16_t>;
	using LineSeqMesh = MeshData<Vertex3d, uint16_t>;
}

#endif