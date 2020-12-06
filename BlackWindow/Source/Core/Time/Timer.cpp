#pragma once
#include "Timer.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "UIWindow/ConsoleWindow.h"
void Timer::Start()
{
    if (currentState == TimerState::RUNNING)
    {
        App->editor->consoleWindow->AddLog("Timer already runnning");
        return;
    }

    time = 0;
    currentState = TimerState::RUNNING;
}

void Timer::Stop() 
{
    currentState = TimerState::IDLE;
}
