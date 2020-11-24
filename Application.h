#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleInput;
class ModuleCamera;
class ModuleProgram;
class ModuleTextures;
class ModuleEditor;
class ModuleWorld;

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
	ModuleProgram* program = nullptr;
	ModuleEditor* editor = nullptr;

	//Exercises
	ModuleWorld* world= nullptr;
	void RequestBrowser(const char* route);

private:

	std::list<Module*> modules;
	float lastFrame;

};

extern Application* App;
