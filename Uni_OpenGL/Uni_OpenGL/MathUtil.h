#pragma once

#include <BULLET/btBulletCollisionCommon.h>
#include <GLM/glm.hpp>

#include <GLM\gtc\quaternion.hpp>

glm::vec3 bulletVecToGLM(const btVector3& vec);
btVector3 glmVecToBullet(const glm::vec3& vec);

glm::quat bulletQuatToGLM(const btQuaternion& quat);
btQuaternion glmQuatToBullet(const glm::quat& quat);

glm::quat directionVecToQuat(const glm::vec3& vec);
btQuaternion directionVecToQuat(const btVector3& vec);

glm::quat fromVecToVec(const glm::vec3& vecA, const glm::vec3& vecB);
btQuaternion fromVecToVec(const btVector3& vecA, const btVector3& vecB);

float mapRange(float num, float inputStart, float inputEnd, float outputStart, float outputEnd);
