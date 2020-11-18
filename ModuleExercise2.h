#pragma once
#include "Module.h"
#include "Model.h"
class ModuleExercise2 :
	public Module
{
public:
	ModuleExercise2();
	~ModuleExercise2();

	bool Init();
	update_status PreUpdate(float deltaTime);
	update_status Update(float deltaTime);
	update_status PostUpdate(float deltaTime);
	bool CleanUp();


private:
	Model *house;

	unsigned int program;

};

