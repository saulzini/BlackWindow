#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"
#include "Core/Time/DeltaTime.h"

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
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleDebugDraw* draw = nullptr;
	ModuleEditor* editor = nullptr;
	DeltaTime deltaTime;

	ModuleScene* scene= nullptr;
	void RequestBrowser(const char* route);

	ConsoleWindow* consoleWindow;

private:
	std::list<Module*> modules;
};

extern Application* App;