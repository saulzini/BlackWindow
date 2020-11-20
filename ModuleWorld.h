#pragma once
#include "Module.h"
#include "Model.h"
class ModuleWorld :
	public Module
{
public:
	ModuleWorld();
	~ModuleWorld();

	bool Init();
	update_status PreUpdate(float deltaTime);
	update_status Update(float deltaTime);
	update_status PostUpdate(float deltaTime);
	bool CleanUp();


private:
	Model* house;
	Model *enemy1;

	unsigned int program;
};

