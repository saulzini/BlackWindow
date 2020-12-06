#pragma once
#include "Timer.h"
#include "SDL.h"

class DeltaTime : public Timer
{
public:
    DeltaTime(float mTime = 0.0f) : Timer(mTime) {
        lastFrame = 0.0f;
        timeScale = 0.0f;
        deltaTimeWorldAdjusted =0.0f;
    }

    void CalculateDeltaTime()
    {
        float currentTime = (float)SDL_GetTicks();
        time = currentTime - lastFrame; //Delta time
        deltaTimeWorldAdjusted = time * timeScale;
        lastFrame = currentTime;
    }

    float GetTimeScale() const{
        return timeScale;
    }

    void SetTimeScale(float scale){
        if (scale == timeScale){
            return;
        }

        timeScale = scale;
    }

    float GetDeltaTimeWorldAdjusted() const{
        return deltaTimeWorldAdjusted;
    }


protected:
    float lastFrame;
    float timeScale;
    float deltaTimeWorldAdjusted;

};
