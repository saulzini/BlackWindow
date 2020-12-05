#pragma once
#include "Timer.h"
#include "Globals.h"
class WorldTimer : public Timer
{
public:
    WorldTimer(float mTime = 0.0f) : Timer(mTime){
        maxFps = 30.0f;
        fps =0;
        lastFrame = 0;
    }

    void Update();
    void CalculateFPS(float previousTicks, float currentTicks);
    void RegulateFPS();

    const float* GetFpsResults() const{
        return fpsResults;
    }

    const float* GetFrameTimesResults() const{
        return frameTimesResults;
    }

protected:
	float maxFps;
    float fps;
    float lastFrame;

	// void CalculateFPS(float previousTicks,float currentTicks);
	// void RegulateFPS(float currentTime);

	float fpsResults[SAMPLESFPS]={0.0f}; //Array for saving fps records
	float frameTimesResults[SAMPLESFPS]={0.0f}; //Array for saving frame times recods
};
