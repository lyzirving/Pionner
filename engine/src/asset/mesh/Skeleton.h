#pragma once

#include "asset/Asset.h"

namespace pio
{
	class Skeleton : public Asset
	{
		RTTR_ENABLE(Asset)
	public:
		Skeleton(const ImportParams& params);
		virtual bool IsInit() const { return !m_Joints.empty(); }

		int32_t GetJointIndex(const std::string& name) const;

		bool HasJoint(const std::string& name) const { return m_JointInfo.find(name) != m_JointInfo.end(); }
		Joint& GetJoint(int32_t index) { return m_Joints[index]; }
		const Joint& GetJoint(int32_t index) const { return m_Joints[index]; }
		uint32_t GetJointSize() const { return m_Joints.size(); }

	private:		
		std::vector<Joint> m_Joints;
		std::unordered_map<std::string, int32_t> m_JointInfo;

		friend class MeshImporter;
	};
}