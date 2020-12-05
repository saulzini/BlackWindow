#pragma once
#include "Module.h"
#include "Core/Model.h"
#include "Core/Time/WorldTimer.h"

class ModuleWorld :
	public Module
{
public:
	ModuleWorld();
	~ModuleWorld();

	bool Init() override;
	update_status PreUpdate(float deltaTime) override;
	update_status Update(float deltaTime) override;
	update_status PostUpdate(float deltaTime) override;
	bool CleanUp() override;

	Model* GetModel() const {
		return model;
	}


	// For testing
	void SwapTexture(const char *texturePath);
	void SwapModel(const char *modelPath);

private:
	Model* model;
	unsigned int program;
	WorldTimer worldTimer;
};

