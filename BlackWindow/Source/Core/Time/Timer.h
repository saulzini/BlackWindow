#pragma once
#include "SDL.h"
#include "Core/Time/TimerState.h"

class Timer
{
public:
    Timer(float mTime = 0.0f) : time(mTime)
    {
        currentState = TimerState::IDLE;
    }

    virtual float GetSeconds() const { return time / 1000.0f; }
    virtual float GetMiliseconds() const { return time; }

    TimerState GetCurrentState() const { return currentState; }

    virtual void Start();

    virtual void Stop();

protected:
    float time;
    TimerState currentState;
};
