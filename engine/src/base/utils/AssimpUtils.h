#pragma once

#include "base/math/MathDef.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/version.h>

namespace pio
{
	namespace AssimpUtils
	{
		inline glm::mat4 Mat4FromAiMatrix4x4(const aiMatrix4x4& matrix)
		{
			glm::mat4 result;
			//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
			result[0][0] = matrix.a1; result[1][0] = matrix.a2; result[2][0] = matrix.a3; result[3][0] = matrix.a4;
			result[0][1] = matrix.b1; result[1][1] = matrix.b2; result[2][1] = matrix.b3; result[3][1] = matrix.b4;
			result[0][2] = matrix.c1; result[1][2] = matrix.c2; result[2][2] = matrix.c3; result[3][2] = matrix.c4;
			result[0][3] = matrix.d1; result[1][3] = matrix.d2; result[2][3] = matrix.d3; result[3][3] = matrix.d4;
			return result;
		}

		inline glm::vec3 Vec3FromAiVector3D(const aiVector3D& vec) 
		{
			return glm::vec3(vec.x, vec.y, vec.z);
		}

		inline glm::quat QuatFromAiQuat(const aiQuaternion& orientation) 
		{
			return glm::quat(orientation.w, orientation.x, orientation.y, orientation.z);
		}
	}
}