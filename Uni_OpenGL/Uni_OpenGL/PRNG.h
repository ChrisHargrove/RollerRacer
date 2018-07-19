#pragma once

#include <random>

class PRNG
{
public:
    PRNG(unsigned int seed = 0, int min = 0, int max = 1) {
        _MersenneTwisterGenerator.seed(seed);
        _RangeMinimum = min;
        _RangeMaximum = max;

        _NumberDistribution = std::uniform_real_distribution<>(_RangeMinimum, _RangeMaximum);
    }
    ~PRNG() {}

    double GetNumber() { return _NumberDistribution(_MersenneTwisterGenerator); }
    float GetNumberF() { return (float)_NumberDistribution(_MersenneTwisterGenerator); }

    void SetSeed(unsigned int seed) { _Seed = seed; }
    unsigned int GetSeed() { return _Seed; }

    void SetRange(int min, int max) {
        _RangeMinimum = min;
        _RangeMaximum = max;

        _NumberDistribution = std::uniform_real_distribution<>(_RangeMinimum, _RangeMaximum);
    }

private:
    std::mt19937 _MersenneTwisterGenerator;
    std::uniform_real_distribution<> _NumberDistribution;

    unsigned int _Seed;
    int _RangeMinimum;
    int _RangeMaximum;
};

