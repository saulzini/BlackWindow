#pragma once
#include "SDL.h"


enum TimerState {
    IDLE,
    RUNNING,
    PAUSE,
};

class Timer
{
    public:
        Timer(float mTime=0.0f): time(mTime){
            currentState = TimerState::IDLE;
        }

        float GetSeconds() const { return time / 1000.0f; }
        float GetMiliseconds() const { return time; }
        virtual void Start(){
            time = 0;
            currentState = TimerState::RUNNING;
        }

        // virtual void Update(){
            // float time = (float) SDL_GetTicks();
        	// currentTime = time-lastFrame; //Delta time
	        // lastFrame = time;
        // }   

        virtual void Stop(){
            currentState = TimerState::IDLE;
        }

    private:
        float time;
        TimerState currentState;

};

