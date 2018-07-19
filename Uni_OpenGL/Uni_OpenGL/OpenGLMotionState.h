#pragma once

#include <GLM\glm.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include <BULLET\btBulletCollisionCommon.h>

class OpenGLMotionState : public btDefaultMotionState
{
public:
    OpenGLMotionState(const btTransform &transform) :
        btDefaultMotionState(transform) {}

    void GetWorldTransform(btScalar* transform) {
        btTransform trans;
        getWorldTransform(trans);
        trans.getOpenGLMatrix(transform);
    }

    glm::mat4 BulletTransformToGLM(btTransform& transform) {
        float data[16];
        transform.getOpenGLMatrix(data);
        return glm::make_mat4(data);
    }

    void GetWorldMatrix(glm::mat4& mat) {
        btTransform trans;
        getWorldTransform(trans);
        mat = BulletTransformToGLM(trans);
    }

    glm::mat4 GetWorldMatrix() {
        btTransform trans;
        getWorldTransform(trans);
        return BulletTransformToGLM(trans);
    }
};

