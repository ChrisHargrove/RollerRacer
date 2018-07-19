#pragma once

#include <vector>
#include <GLM\glm.hpp>

#include "PrimitiveShape.h"

class TrackGenerator : public PrimitiveShape
{
public:
    TrackGenerator();
    ~TrackGenerator();

    void GeneratePoints();
    void MonotoneChainAlgorithm();
    void PushPointsApart();
    void AddDifficulty();
    void FixAngles();
	void IncreaseSize(float size);

    void SetSeed(int seed) { _Seed = seed; }
    void SetDifficulty(float difficulty) { _Difficulty = difficulty; }

	std::vector<glm::vec2>* GetPoints();

    void AddToBuffer();
    void Render(std::string shader = "");

private:
    int _Seed = 0;
    int _PushIterations = 3;
    float _Difficulty = 1.0f;
    float _MaxDisplacement = 20.0f;

	glm::vec2 _GenerationOffset;

    std::vector<glm::vec2> _Points;

    double Cross(const glm::vec2& O, const glm::vec2& A, const glm::vec2& B);
};

