#pragma once
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleScene.h"
#include "ModuleEditor.h"
#include "ModuleDebugDraw.h"
#include "SDL.h"
#include "Leaks.h"
#include <IL/ilut.h>
#include "UIWindow/ConsoleWindow.h" 
using namespace std;

Application::Application()
{
	// init DevIL. This needs to be done only once per application
	//TODO::IMPROVE THIS
	ilInit();
	iluInit();
	lcg = new LCG();

	// Order matters: they will Init/start/update in this order
	modules.push_back(window = new ModuleWindow());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(input = new ModuleInput());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(draw = new ModuleDebugDraw());
	modules.push_back(scene = new ModuleScene());
	modules.push_back(editor = new ModuleEditor());
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

	deltaTime.Start();

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	deltaTime.CalculateDeltaTime();
	float currentTime = deltaTime.GetMiliseconds();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate(currentTime);

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it){

		// if ( (*it) == scene || (*it) == camera){ //Testing purposes at the end only let the world
		if ( (*it) == scene){
			ret = (*it)->Update( deltaTime.GetDeltaTimeWorldAdjusted() );
		}
		else {
			ret = (*it)->Update(currentTime);
		}
	}

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate(currentTime);

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