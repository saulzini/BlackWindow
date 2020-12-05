#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"
// #include "Core/Time.h"
#include "Core/Time/DeltaTime.h"

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
	DeltaTime deltaTime;

	//Exercises
	ModuleWorld* world= nullptr;
	void RequestBrowser(const char* route);

	// Time GetTime() const{
	// 	return currentTime;
	// }

	float GetMaxFps() const{
		return maxFps;
	}

	void SetMaxFps(float value){
		if (maxFps == value ){
			return;
		}
		maxFps = value;
	}

	float* GetFpsResults(){
		return fpsResults;
	}

	float* GetFrameTimes(){
		return frameTimesResults;
	}

private:

	std::list<Module*> modules;
	// float lastFrame;
	// Time currentTime;

	float fps;
	float maxFps;
	void CalculateFPS(float previousTicks,float currentTicks);
	void RegulateFPS(float currentTime);

	float fpsResults[SAMPLESFPS]={0.0f}; //Array for saving fps records
	float frameTimesResults[SAMPLESFPS]={0.0f}; //Array for saving frame times recods
	
};

extern Application* App;