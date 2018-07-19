#include "MathUtil.h"

glm::vec3 bulletVecToGLM(const btVector3 & vec)
{
	return glm::vec3(vec.x(), vec.y(), vec.z());
}

btVector3 glmVecToBullet(const glm::vec3 & vec)
{
	return btVector3(vec.x, vec.y, vec.z);
}

glm::quat bulletQuatToGLM(const btQuaternion & quat)
{
	return glm::quat(quat.w(), quat.x(), quat.y(), quat.z());
}

btQuaternion glmQuatToBullet(const glm::quat & quat)
{
	return btQuaternion(quat.x, quat.y, quat.z, quat.w);
}

glm::quat directionVecToQuat(const glm::vec3 & vec)
{
	return glm::quat();
}

btQuaternion directionVecToQuat(const btVector3 & vec)
{
	return btQuaternion();
}

glm::quat fromVecToVec(const glm::vec3 & vecA, const glm::vec3 & vecB)
{
	float norm_u_v = sqrt(glm::dot(vecA, vecA) * glm::dot(vecB, vecB));
	float real_part = norm_u_v + glm::dot(vecA, vecB);
	glm::vec3 w;

	if (real_part < 1.e-6f * norm_u_v)
	{
		/* If u and v are exactly opposite, rotate 180 degrees
		* around an arbitrary orthogonal axis. Axis normalisation
		* can happen later, when we normalise the quaternion. */
		real_part = 0.0f;
		w = abs(vecA.x) > abs(vecA.z) ? glm::vec3(-vecA.y, vecA.x, 0.f)
			: glm::vec3(0.f, -vecA.z, vecA.y);
	}
	else
	{
		/* Otherwise, build quaternion the standard way. */
		w = glm::cross(vecA, vecB);
	}

	return glm::normalize(glm::quat(real_part, w.x, w.y, w.z));
}

btQuaternion fromVecToVec(const btVector3 & vecA, const btVector3 & vecB)
{
	return glmQuatToBullet(fromVecToVec(bulletVecToGLM(vecA), bulletVecToGLM(vecB)));
}

float mapRange(float num, float inputStart, float inputEnd, float outputStart, float outputEnd)
{
	return ((num - inputStart) * ((outputEnd - outputStart) / (inputEnd - inputStart))) + outputStart;
}
