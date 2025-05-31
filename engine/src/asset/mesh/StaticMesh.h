#pragma once

#include "asset/Asset.h"

namespace pio
{
	class Material;

	class StaticMesh : public Asset
	{
		RTTR_ENABLE(Asset)
	public:
		StaticMesh(const ImportParams& params);

		virtual bool IsInit() const override { return !m_SubMeshes.empty(); }
		virtual void OnInit() override;
		virtual Ref<Asset> SpawnChild() override;
		virtual bool IsSkinned() const { return false; }		

		std::vector<SubMesh>& GetSubMesh() { return m_SubMeshes; }
		const std::vector<SubMesh>& GetSubMesh() const { return m_SubMeshes; }

		std::vector<Ref<Material>>& GetMaterial() { return m_Materials; }
		const std::vector<Ref<Material>>& GetMaterial() const { return m_Materials; }

		SubMeshNode& GetNodeRoot() { return m_Root; }
		const SubMeshNode& GetNodeRoot() const { return m_Root; }

	protected:
		std::vector<SubMesh> m_SubMeshes;
		std::vector<Ref<Material>> m_Materials;
		SubMeshNode m_Root;

	private:
		friend class MeshImporter;
		friend class StaticMeshComponent;
	};
}