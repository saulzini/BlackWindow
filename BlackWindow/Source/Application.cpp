#pragma once
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "ModuleWorld.h"
#include "ModuleEditor.h"
#include "Core/Time.h"
#include "SDL.h"
#include "Leaks.h"
#include <IL/ilut.h> 

using namespace std;

Application::Application()
{
	// init DevIL. This needs to be done only once per application
	//TODO::IMPROVE THIS
	ilInit();
	iluInit();

	// Order matters: they will Init/start/update in this order
	modules.push_back(window = new ModuleWindow());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(input = new ModuleInput());
	modules.push_back(program = new ModuleProgram());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(editor = new ModuleEditor());

	modules.push_back(world = new ModuleWorld());

	// FPS
	maxFps = 60;
	lastFrame = 0;
	fps = 0;
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	// Used for time measuring
	float time = (float) SDL_GetTicks();
	CalculateFPS(lastFrame,time); //Calculating FPS
	currentTime = time-lastFrame; //Delta time
	lastFrame = time;
	

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate(currentTime);

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update(currentTime);

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate(currentTime);


	RegulateFPS(time); //Regulate the frame given the max fps

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}

void Application::RequestBrowser(const char* route)
{
	ShellExecute(NULL, "open", route, nullptr, nullptr, SW_SHOWNORMAL);
}

void Application::CalculateFPS(float previousTicks,float currentTicks) 
{
	static int currentFrame = 0;

	float frameTime = currentTicks -previousTicks;

	int index = currentFrame % SAMPLESFPS;
	frameTimesResults[index] = frameTime; //Saving the frame time
	fpsResults[index] = fps; //Saving the fps time

	int count;
	currentFrame++;

	if (currentFrame < SAMPLESFPS){
		count = currentFrame;
	}
	else {
		count = SAMPLESFPS;
	}

	float frameTimeAverage = 0;
	// getting average
	for(unsigned int i=0; i<count; i++){ 
		frameTimeAverage += frameTimesResults[i];
	}

	frameTimeAverage /=count;
	// Getting fps
	if (frameTimeAverage > 0){
		fps = 1000.0f/frameTimeAverage;
	}
	else {
		fps = 0;
	}
}

void Application::RegulateFPS(float currentTime) 
{
	float frameTicks = SDL_GetTicks()-currentTime;
	if (1000.0f / maxFps > frameTicks){
		// force frame to delay
		SDL_Delay(1000.0f / (maxFps - frameTicks));
	}
}
