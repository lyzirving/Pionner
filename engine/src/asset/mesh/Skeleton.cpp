#include "Skeleton.h"

namespace pio
{
	Skeleton::Skeleton(const ImportParams& params) : Asset(params)
	{
	}

	int32_t Skeleton::GetJointIndex(const std::string& name) const
	{
		auto it = m_JointInfo.find(name);
		if(it != m_JointInfo.end())
		{
			return it->second;
		}
		return -1;
	}
}