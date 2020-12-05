#pragma once
#include "WorldTimer.h"
#include "SDL.h"

void WorldTimer::CalculateFPS(float previousTicks, float currentTicks)
{
    static int currentFrame = 0;

    float frameTime = currentTicks - previousTicks;

    int index = currentFrame % SAMPLESFPS;
    frameTimesResults[index] = frameTime; //Saving the frame time
    fpsResults[index] = fps;              //Saving the fps time

    int count;
    currentFrame++;

    if (currentFrame < SAMPLESFPS)
    {
        count = currentFrame;
    }
    else
    {
        count = SAMPLESFPS;
    }

    float frameTimeAverage = 0;
    // getting average
    for (unsigned int i = 0; i < count; i++)
    {
        frameTimeAverage += frameTimesResults[i];
    }

    frameTimeAverage /= count;
    // Getting fps
    if (frameTimeAverage > 0)
    {
        fps = 1000.0f / frameTimeAverage;
    }
    else
    {
        fps = 0;
    }
}

void WorldTimer::Update() 
{
    float currentTime = (float) SDL_GetTicks();
	CalculateFPS(lastFrame,currentTime); //Calculating FPS
	currentTime = currentTime-lastFrame; //Delta time
	lastFrame = currentTime;
}

void WorldTimer::RegulateFPS()
{
    float frameTicks = SDL_GetTicks() - lastFrame;
    if (1000.0f / maxFps > frameTicks)
    {
        // force frame to delay
        SDL_Delay(1000.0f / (maxFps - frameTicks));
    }
}