#include <math.h>

#include "MathLib.h"

namespace pio
{
	namespace Math
	{
		bool DecomposeTransform(const glm::mat4 &transform, glm::vec3 &translation, glm::quat &rotation, glm::vec3 &scale)
		{
			glm::mat4 localMatrix(transform);

			if (glm::epsilonEqual(localMatrix[3][3], static_cast<float>(0), glm::epsilon<float>()))
				return false;

			// Next take care of translation (easy).
			translation = glm::vec3(localMatrix[3]);
			localMatrix[3] = glm::vec4(0, 0, 0, localMatrix[3].w);

			// Now get scale and shear.
			glm::vec3 row[3];
			for (size_t i = 0; i < 3; ++i)
				for (size_t j = 0; j < 3; ++j)
					row[i][j] = localMatrix[i][j];

			// Compute X scale factor and normalize first row.
			scale.x = glm::length(row[0]);
			row[0] = Scale(row[0], static_cast<float>(1));
			// Now, compute Y scale and normalize 2nd row.
			scale.y = glm::length(row[1]);
			row[1] = Scale(row[1], static_cast<float>(1));
			// Next, get Z scale and normalize 3rd row.
			scale.z = glm::length(row[2]);
			row[2] = Scale(row[2], static_cast<float>(1));

			// Rotation as quaternion
			int i, j, k = 0;
			float root, trace = row[0].x + row[1].y + row[2].z;
			if (trace > static_cast<float>(0))
			{
				root = sqrt(trace + static_cast<float>(1));
				rotation.w = static_cast<float>(0.5) * root;
				root = static_cast<float>(0.5) / root;
				rotation.x = root * (row[1].z - row[2].y);
				rotation.y = root * (row[2].x - row[0].z);
				rotation.z = root * (row[0].y - row[1].x);
			} // End if > 0
			else
			{
				static int next[3] = { 1, 2, 0 };
				i = 0;
				if (row[1].y > row[0].x) i = 1;
				if (row[2].z > row[i][i]) i = 2;
				j = next[i];
				k = next[j];

				root = glm::sqrt(row[i][i] - row[j][j] - row[k][k] + static_cast<float>(1.0));

				rotation[i] = static_cast<float>(0.5) * root;
				root = static_cast<float>(0.5) / root;
				rotation[j] = root * (row[i][j] + row[j][i]);
				rotation[k] = root * (row[i][k] + row[k][i]);
				rotation.w = root * (row[j][k] - row[k][j]);
			} // End if <= 0

			return true;
		}
	}
}