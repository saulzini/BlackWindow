#pragma once
#include "Timer.h"
#include "SDL.h"

class DeltaTime : public Timer
{
public:
    DeltaTime(float mTime = 0.0f) : Timer(mTime) {
        lastFrame = 0.0f;
    }

    void CalculateDeltaTime()
    {
        float currentTime = (float)SDL_GetTicks();
        time = currentTime - lastFrame; //Delta time
        lastFrame = currentTime;
    }

protected:
    float lastFrame;
};
