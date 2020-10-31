#pragma once
#include "Module.h"
#include "Globals.h"

#define DEGTORAD(angleDegrees) ((angleDegrees) * M_PI / 180.0)
class ModuleCamera :
	public Module
{

public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
private:
	//Frustum frustum;
};

