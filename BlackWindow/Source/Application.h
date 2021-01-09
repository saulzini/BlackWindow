#pragma once

#include <list>
#include "Globals.h"
#include "Module.h"
#include "Core/Time/DeltaTime.h"
#include "Algorithm/Random/LCG.h"
#include "Core/ResourcesManager/ResourcesManager.h"

class ModuleRender;
class ModuleWindow;
class ModuleInput;
class ModuleCamera;
class ModuleProgram;
class ModuleTextures;
class ModuleEditor;
class ModuleScene;
class ModuleDebugDraw;
class ConsoleWindow;

class Application
{
public:
	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender *renderer = nullptr;
	ModuleWindow *window = nullptr;
	ModuleInput *input = nullptr;
	ModuleCamera *camera = nullptr;
	ModuleDebugDraw *draw = nullptr;
	ModuleEditor *editor = nullptr;
	DeltaTime deltaTime;

	ModuleScene *scene = nullptr;
	void RequestBrowser(const char *route);

	LCG *GetLcg() const
	{
		return lcg;
	}

	ResourcesManager GetResourcesManager(){
		return resourceManager;
	}

private:
	std::list<Module *> modules;
	LCG *lcg;
	ResourcesManager resourceManager;

};

extern Application *App;