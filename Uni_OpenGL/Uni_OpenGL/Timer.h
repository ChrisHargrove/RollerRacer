#pragma once

#include <chrono>

class Timer
{
public:
    Timer() {};
    ~Timer() {};

    void Start() { _Start = std::chrono::high_resolution_clock::now(); }
    void End() { _End = std::chrono::high_resolution_clock::now(); }
    float GetDelta() { 
        End();
        _ElapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(_End - _Start);
        Start();
        return _ElapsedTime.count();
    }

private:
    std::chrono::duration<float> _ElapsedTime;
    std::chrono::high_resolution_clock::time_point _Start;
    std::chrono::high_resolution_clock::time_point _End;
};

