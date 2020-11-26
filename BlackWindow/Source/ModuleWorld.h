#pragma once
#include "Module.h"
#include "Core/Model.h"
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

	Model* GetModel() const {
		return model;
	}

	void SwapModel(const char *modelPath);

private:
	Model* model;
	unsigned int program;
};

