#pragma once
#include "Module.h"

class ModuleExercise2 :
	public Module
{
public:
	ModuleExercise2();
	~ModuleExercise2();

	unsigned int getProgram() const {
		return program;
	};

	unsigned int getVbo() const {
		return vbo;
	};

	bool Init();
	update_status PreUpdate(float deltaTime);
	update_status Update(float deltaTime);
	update_status PostUpdate(float deltaTime);
	bool CleanUp();



private:
	unsigned int program;
	unsigned int vbo;
	unsigned int texture;
};

