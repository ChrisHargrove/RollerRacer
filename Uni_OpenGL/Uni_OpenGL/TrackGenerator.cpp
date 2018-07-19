#include "TrackGenerator.h"
#include "PRNG.h"

#include "ShaderManager.h"

#include <math.h>
#include <GLM\gtc\constants.hpp>
#include <GLM\gtx\rotate_vector.hpp>
#include <GLM\gtx\vector_angle.hpp>

//https://www.gamasutra.com/blogs/GustavoMaciel/20131229/207833/Generating_Procedural_Racetracks.php

TrackGenerator::TrackGenerator()
{
}


TrackGenerator::~TrackGenerator()
{
}

void TrackGenerator::GeneratePoints()
{
    _Points.clear();
	_GenerationOffset = glm::vec2(125, 125);

    PRNG _NumberGenerator(_Seed, 10, 20);

    int _NumberOfPoints = (int)_NumberGenerator.GetNumber();

    _NumberGenerator.SetRange(0, 250);

    for (int i = 0; i < _NumberOfPoints; i++) {
		float x = _NumberGenerator.GetNumberF() - _GenerationOffset.x;
		float z = _NumberGenerator.GetNumberF() - _GenerationOffset.y;
        _Points.push_back(glm::vec2(x, z));
    }

    MonotoneChainAlgorithm();
    AddDifficulty();

	IncreaseSize(50.0f);

    AddToBuffer();
}

void TrackGenerator::MonotoneChainAlgorithm()
{
    //explanation of algorithm
    //https://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain


    //sort all the points by ascending value of x, if x values are the same the compare the y values;
    std::sort(_Points.begin(), _Points.end(), [](glm::vec2 a, glm::vec2 b) { return a.x < b.x || ( a.x == b.x && a.y < b.y); });

    //stores all points for the hull
    std::vector<glm::vec2> _HullPoints(2 * _Points.size());
    int k = 0; //unsure about k atm

    //builds upper hull
    for (int i = 0; i < (int)_Points.size(); ++i) {
        while (k >= 2 && Cross(_HullPoints[k - 2], _HullPoints[k - 1], _Points[i]) <= 0) {
            k--;
        }
        _HullPoints[k++] = _Points[i];
    }

    //builds lower hull
    for (int i = _Points.size() - 1, t = k + 1; i > 0; --i) {
        while (k >= t && Cross(_HullPoints[k - 2], _HullPoints[k - 1], _Points[i - 1]) <= 0) {
            k--;
        }
        _HullPoints[k++] = _Points[i - 1];
    }

    _HullPoints.resize(k - 1);

    _Points = _HullPoints;

    for (int i = 0; i < _PushIterations; i++) {
        PushPointsApart();
    }
}

void TrackGenerator::PushPointsApart()
{
    float distance = 15.0f;

    for (int i = 0; i < (int)_Points.size(); ++i) {
        for (int j = i + 1; j < (int)_Points.size(); ++j) {
            //if the distance between the two points is less then threshold
            glm::vec2 temp = glm::vec2(_Points[j] - _Points[i]);
            float tempLength = glm::length(temp);

            if (tempLength < distance) {
                temp = glm::normalize(temp);
                float diff = distance - tempLength;
                temp *= diff;
                _Points[j] += temp;
                _Points[i] -= temp;
            }
        }
    }
}

void TrackGenerator::AddDifficulty()
{
    std::vector<glm::vec2> _ModifiedTrack(_Points.size() * 2);
    glm::vec2 _Displacement;

    PRNG _NumberGenerator(_Seed, 0, 1);

    for (int i = 0; i < (int)_Points.size(); i++) {
        float dispLen = pow(_NumberGenerator.GetNumberF(), _Difficulty) * _MaxDisplacement;
        glm::vec2 disp(0, 1);
        disp = glm::rotate(disp, _NumberGenerator.GetNumberF() * 360);
        disp *= dispLen;
        _ModifiedTrack[i * 2] = _Points[i];
        _ModifiedTrack[i * 2 + 1] = _Points[i];
        _ModifiedTrack[i * 2 + 1] += (_Points[(i + 1) % _Points.size()] / 2.0f) + disp;
    }
    _Points = _ModifiedTrack;

    for (int i = 0; i < _PushIterations; ++i) {
        PushPointsApart();
    }
}

void TrackGenerator::FixAngles()
{
    for (int i = 0; i < (int)_Points.size(); i++) {
        int previous = (i - 1 < 0) ? _Points.size() - 1 : i - 1;
        int next = (i + 1) % _Points.size();

        glm::vec2 PrevPoint( _Points[i] - _Points[previous]);
        PrevPoint = glm::normalize(PrevPoint);
        glm::vec2 NextPoint(_Points[i] - _Points[next]);
        NextPoint.x = -NextPoint.x;
        NextPoint.y = -NextPoint.y;
        float nextLength = glm::length(NextPoint);
        NextPoint = glm::normalize(NextPoint);

        float a = atan2(((PrevPoint.x * NextPoint.y) - (PrevPoint.y * NextPoint.x)), ((PrevPoint.x * NextPoint.x) + (PrevPoint.y * NextPoint.y)));
        if (abs(a * 180 / glm::pi<float>()) <= 100) continue;

        float nA = (100 * glm::sign(a)) * 180 / glm::pi<float>();
        float diff = nA - a;
        float cos = cosf(diff);
        float sin = sinf(diff);
        float newX = NextPoint.x * cos - NextPoint.y * sin;
        float newY = NextPoint.x * sin + NextPoint.y * cos;
        newX *= nextLength;
        newY *= nextLength;
        _Points[next].x = _Points[i].x + newX;
        _Points[next].y = _Points[i].y + newY;
    }
}

void TrackGenerator::IncreaseSize(float size)
{
	//need to push point further away from center track
	//to make the tracks larger but stay the same shape.
	for (int i = 0; i < (int)_Points.size(); i++) {
		glm::vec2 direction = glm::normalize(_Points[i]);
		_Points[i] = _Points[i] + (direction * size);
	}

	//for (auto p : _Points) {
	//	
	//	glm::vec2 pointDir = glm::normalize(p);
	//	p += pointDir * size;
	//}
}

std::vector<glm::vec2>* TrackGenerator::GetPoints()
{
	return &_Points;
}

void TrackGenerator::AddToBuffer()
{
    //_Points.push_back(_Points[0]);

    std::vector<float> points;
    for (auto p : _Points) {
        points.push_back(p.x);
        points.push_back(0);
        points.push_back(p.y);

        points.push_back(0);
        points.push_back(1);
        points.push_back(0);
    }

    _VertexArray.Create(VAO);
    _VertexBuffer.Create(VBO);
    _VertexArray.Bind();
    _VertexBuffer.Fill(sizeof(float) * points.size(), &points[0], STATIC);

    _VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader("basic")->GetID(), "aPos", 3, VT_FLOAT, 6 * sizeof(float));
    _VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader("basic")->GetID(), "aNormal", 3, VT_FLOAT, 6 * sizeof(float), 3 * sizeof(float));
    _VertexArray.Unbind();

}

void TrackGenerator::Render(std::string shader)
{
    ShaderManager::Instance()->GetShader("basic")->SetVec3("aColor", glm::vec3(0.0, 0.0, 1.0));
    glLineWidth(10);
    _VertexArray.Bind();
        glDrawArrays(GL_LINE_STRIP, 0, _Points.size());
    _VertexArray.Unbind();
}

double TrackGenerator::Cross(const glm::vec2 & O, const glm::vec2 & A, const glm::vec2 & B)
{
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}
