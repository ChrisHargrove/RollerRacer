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

class StopWatch
{
public:
	StopWatch() { _IsTiming = false; };
	~StopWatch() {};

	void Start() { 
		_Start = std::chrono::high_resolution_clock::now(); 
		_IsTiming = true;
	}
	void End() { _End = std::chrono::high_resolution_clock::now(); }
	void Reset() { _TotalElapsedTime = 0.0f; }
	void Update() { 
		if (_IsTiming) {
			End();
			_ElapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(_End - _Start);
			Start();
			_TotalElapsedTime += _ElapsedTime.count();
		}
	}
	void Pause() {
		_IsTiming = false;
	}
	void Resume() {
		End();
		Start();
		_IsTiming = true;
	}

	float GetElapsedTime() { return _TotalElapsedTime; }
	float IsTiming() { return _IsTiming; }

private:
	float _TotalElapsedTime = 0.0f;
	bool _IsTiming = false;

	std::chrono::duration<float> _ElapsedTime;
	std::chrono::high_resolution_clock::time_point _Start;
	std::chrono::high_resolution_clock::time_point _End;
};

