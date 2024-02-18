#ifndef __PIONNER_ASSET_ASSIMP_UTILS_H__
#define __PIONNER_ASSET_ASSIMP_UTILS_H__

#include <assimp/types.h>

#include "core/Base.h"

namespace pio
{
	class AssimpUtils
	{
	public:
		static glm::mat4 Mat4FromAiMatrix4x4(const aiMatrix4x4 &matrix);
		static inline glm::vec3 Vec3FromAiVector3D(const aiVector3D &vec) { return glm::vec3(vec.x, vec.y, vec.z); }
		static inline glm::quat QuatFromAiQuat(const aiQuaternion &orientation) { return glm::quat(orientation.w, orientation.x, orientation.y, orientation.z); }

	private:
		AssimpUtils() {}
		~AssimpUtils() = default;
	};
}

#endif