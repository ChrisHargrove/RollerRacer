#include "CatmullRomSpline.h"
#include <math.h>

#include "ShaderManager.h"

//https://en.wikipedia.org/wiki/Centripetal_Catmull%E2%80%93Rom_spline


CatmullRomSpline::CatmullRomSpline()
{
	_VertexArray.Create(VAO);
	_VertexBuffer.Create(VBO);
}

CatmullRomSpline::CatmullRomSpline(std::vector<glm::vec2> points, float numCurvePoints)
{
	//set original data set
	_Points = points;
	//get number of points to generate per curve
	_PointCountPerCurve = numCurvePoints;
	//reserve memory the new spline points
	_SplinePoints.reserve((unsigned int)(_Points.size() * _PointCountPerCurve));

	GenerateSpline();
}


CatmullRomSpline::~CatmullRomSpline()
{
}

void CatmullRomSpline::GenerateSpline()
{
	_SplinePoints.clear();
	//for each curve
	for (int i = 0; i < (int)_Points.size(); i++) {
		int previous = (i - 1 < 0) ? _Points.size() - 1 : i - 1;
		int next = (i + 1) % _Points.size();

		//get a point either side of the pounts your generating a curve from and to.
		glm::vec2 p0(_Points[previous]);
		glm::vec2 p1(_Points[i]);
		glm::vec2 p2(_Points[next]);
		glm::vec2 p3(_Points[(next + 1) % _Points.size()] );

		//this is the part that does the interpolation across the spline.
		float t0 = 0.0f;
		float t1 = GetT(t0, p0, p1);
		float t2 = GetT(t1, p1, p2);
		float t3 = GetT(t2, p2, p3);

		for (float t = t1; t < t2; t += ((t2 - t1) / _PointCountPerCurve)) {
			glm::vec2 A1 = (t1 - t) / (t1 - t0) * p0 + (t - t0) / (t1 - t0) * p1;
			glm::vec2 A2 = (t2 - t) / (t2 - t1) * p1 + (t - t1) / (t2 - t1) * p2;
			glm::vec2 A3 = (t3 - t) / (t3 - t2) * p2 + (t - t2) / (t3 - t2) * p3;

			glm::vec2 B1 = (t2 - t) / (t2 - t0) * A1 + (t - t0) / (t2 - t0) * A2;
			glm::vec2 B2 = (t3 - t) / (t3 - t1) * A2 + (t - t1) / (t3 - t1) * A3;

			glm::vec2 C = (t2 - t) / (t2 - t1) * B1 + (t - t1) / (t2 - t1) * B2;

			_SplinePoints.push_back(C);
		}
	}
}

float CatmullRomSpline::GetT(float t, glm::vec2 p0, glm::vec2 p1)
{
	float a = pow((p1.x - p0.x), 2.0f) + pow((p1.y - p0.y), 2.0f);
	float b = pow(a, 0.5f);
	float c = pow(b, _Alpha);

	return (c + t);
}

void CatmullRomSpline::AddToBuffer()
{
	//allows the spline to be rendered to the screen.
	std::vector<float> points;
	for (auto p : _SplinePoints) {
		points.push_back(p.x);
		points.push_back(2.0f);
		points.push_back(p.y);

		points.push_back(0);
		points.push_back(1);
		points.push_back(0);
	}

	
	_VertexArray.Bind();
	_VertexBuffer.Fill(sizeof(float) * points.size(), &points[0], STATIC);

	_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader("basic")->GetID(), "aPos", 3, VT_FLOAT, 6 * sizeof(float));
	_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader("basic")->GetID(), "aNormal", 3, VT_FLOAT, 6 * sizeof(float), 3 * sizeof(float));
	_VertexArray.Unbind();
}

void CatmullRomSpline::AddToBuffer2D()
{
	//creates the spline and adds to a vertex buffer set up to be drawn in ortho projection.
	std::vector<float> points;
	for (auto p : _SplinePoints) {
		points.push_back(p.x);
		points.push_back(p.y);
		points.push_back(0.0f);

		points.push_back(0);
		points.push_back(0);
		points.push_back(1);
	}

	_VertexArray.Bind();
	_VertexBuffer.Fill(sizeof(float) * points.size(), &points[0], STATIC);

	_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader("basic")->GetID(), "aPos", 3, VT_FLOAT, 6 * sizeof(float));
	_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader("basic")->GetID(), "aNormal", 3, VT_FLOAT, 6 * sizeof(float), 3 * sizeof(float));
	_VertexArray.Unbind();
}

void CatmullRomSpline::Render(std::string shader)
{
	glLineWidth(3.0f); //increase line width so its visible easier.
	ShaderManager::Instance()->GetShader("basic")->SetVec3("aColor", glm::vec3(1.0, 0.0, 0.0));
	_VertexArray.Bind();
	glDrawArrays(GL_LINE_STRIP, 0, _SplinePoints.size());
	_VertexArray.Unbind();
	glLineWidth(1.0f);
}
