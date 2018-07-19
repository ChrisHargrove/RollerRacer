#pragma once

#include <BULLET\btBulletCollisionCommon.h>
#include <BULLET\btBulletDynamicsCommon.h>
#include <string>
#include <vector>

#include "Buffer.h"
#include "ShaderManager.h"

class OpenGLDebugDrawer : public btIDebugDraw
{
public:
    OpenGLDebugDrawer() {
        _Shader = "debug";
        _VertexArray.Create(VAO);
        _VertexBuffer.Create(VBO);
        _VertexArray.Bind();

        _VertexBuffer.Bind();
        _VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader(_Shader)->GetID(), "aPos", 3, VT_FLOAT, 6 * sizeof(float));
        _VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader(_Shader)->GetID(), "aColor", 3, VT_FLOAT, 6 * sizeof(float), 3 * sizeof(float));

        _VertexArray.Unbind();
    }

    virtual ~OpenGLDebugDrawer() {
        _VertexArray.Destroy();
        _VertexBuffer.Destroy();
    }


    virtual void setDebugMode(int debugMode) override {
        _DebugMode = debugMode;
    }
    virtual int	getDebugMode() const override {
        return _DebugMode;
    }

    virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override {
        //draws a line between contact points
        btVector3 const startPoint = PointOnB;
        btVector3 const endPoint = PointOnB + normalOnB * distance;
        drawLine(startPoint, endPoint, color);
    }

    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override {
        _DebugLines.push_back(from.x()); _DebugLines.push_back(from.y()); _DebugLines.push_back(from.z());
        _DebugLines.push_back(color.x()); _DebugLines.push_back(color.y()); _DebugLines.push_back(color.z());

        _DebugLines.push_back(to.x()); _DebugLines.push_back(to.y()); _DebugLines.push_back(to.z());
        _DebugLines.push_back(color.x()); _DebugLines.push_back(color.y()); _DebugLines.push_back(color.z());
    }

	virtual void drawTriangle(const btVector3& v0, const btVector3& v1, const btVector3& v2, const btVector3& color, btScalar) {
		_DebugTriangles.push_back(v0.x()); _DebugTriangles.push_back(v0.y()); _DebugTriangles.push_back(v0.z());
		_DebugTriangles.push_back(color.x()); _DebugTriangles.push_back(color.y()); _DebugTriangles.push_back(color.z());

		_DebugTriangles.push_back(v1.x()); _DebugTriangles.push_back(v1.y()); _DebugTriangles.push_back(v1.z());
		_DebugTriangles.push_back(color.x()); _DebugTriangles.push_back(color.y()); _DebugTriangles.push_back(color.z());

		_DebugTriangles.push_back(v2.x()); _DebugTriangles.push_back(v2.y()); _DebugTriangles.push_back(v2.z());
		_DebugTriangles.push_back(color.x()); _DebugTriangles.push_back(color.y()); _DebugTriangles.push_back(color.z());
	}

    //Not Used But Are Pure Virtual
    virtual void reportErrorWarning(const char* warningString) override {}
    virtual void draw3dText(const btVector3& location, const char* textString) override {}

    void ToggleDebugFlag(int flag) {
        //toggles debug flags
        if (_DebugMode & flag) {
            //if enabled then disable
            _DebugMode = _DebugMode & (~flag);
        }
        else {
            //if disabled then enable
            _DebugMode |= flag;
        }
    }

    void Render() {
        _VertexBuffer.Fill(sizeof(float) * _DebugLines.size(), &_DebugLines[0], DYNAMIC);
        _VertexArray.Bind();
			glDrawArrays(GL_LINES, 0, _DebugLines.size() /6);
        _VertexArray.Unbind();

		_VertexBuffer.Fill(sizeof(float) * _DebugTriangles.size(), &_DebugTriangles[0], DYNAMIC);
		_VertexArray.Bind();
			glDrawArrays(GL_TRIANGLES, 0, _DebugTriangles.size() /6);
		_VertexArray.Unbind();


        _DebugLines.clear();
        _VertexBuffer.Reset();
    }

private:
    int _DebugMode;
    std::string _Shader;

    std::vector<float> _DebugLines;
	std::vector<float> _DebugTriangles;

    Buffer _VertexArray;
    Buffer _VertexBuffer;

};

