#pragma once

#include <vector>
#include <GLM\glm.hpp>

#include "PrimitiveShape.h"

class CatmullRomSpline : public PrimitiveShape
{
public:
	CatmullRomSpline();

	CatmullRomSpline(std::vector<glm::vec2> points, float numCurvePoints);

	~CatmullRomSpline();

	void GenerateSpline();

	float GetT(float t, glm::vec2 p0, glm::vec2 p1);

	void SetOriginalPoints(std::vector<glm::vec2> points) { _Points.clear(); _Points = points; }
	void SetCurvePointCount(float amount) { _PointCountPerCurve = amount; }
	int GetCurvePointCount() { return (int)_PointCountPerCurve; }

	std::vector<glm::vec2> GetSpline() { return _SplinePoints; }

	void AddToBuffer();
	void AddToBuffer2D();
	void Render(std::string shader = "");

private:
	float _PointCountPerCurve;

	//Set Between 0 - 1;
	float _Alpha = 0.5f;

	std::vector<glm::vec2> _Points;
	std::vector<glm::vec2> _SplinePoints;
};

