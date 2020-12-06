#pragma once
#include "Timer.h"
#include "Globals.h"
class WorldTimer : public Timer
{
public:
    WorldTimer(float mTime = 0.0f) : Timer(mTime){
        maxFps = 60.0f;
        fps =0.0f;
        lastFrame = 0.0f;
        currentTime = 0.0f;
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

    float GetMaxFps() const{
        return maxFps;
    }

    void SetMaxFps(float max){
        if (maxFps == max){
            return;
        }

        maxFps = max;
    }

protected:
	float maxFps;
    float fps;
    float lastFrame;
    float currentTime;

	float fpsResults[SAMPLESFPS]={0.0f}; //Array for saving fps records
	float frameTimesResults[SAMPLESFPS]={0.0f}; //Array for saving frame times recods
};
