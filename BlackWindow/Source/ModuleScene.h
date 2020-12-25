#pragma once
#include "Module.h"
#include "Core/Time/WorldTimer.h"
#include "Core/Skybox.h"
#include "Core/GameObject/GameObjectTypes.h"
class GameObject;

class ModuleScene : public Module
{

private:
	//unsigned int programSky;
	unsigned int program;
	WorldTimer* worldTimer;
	GameObject* root;
	GameObject* selected;

public:
	ModuleScene();
	~ModuleScene();
	unsigned int programSky;
	bool Init() override;
	update_status PreUpdate(float deltaTime) override;
	update_status Update(float deltaTime) override;
	update_status PostUpdate(float deltaTime) override;
	bool CleanUp() override;

	Skybox* GetSkyBox() const {
		return sky;
	}

	GameObject* GetRoot() const {
		return root;
	}

	GameObject* GetSelected() const {
		return selected;
	}

	void SetSelected(GameObject *gameObject){
		selected = gameObject;
	}


	// For testing
	Skybox* sky;
	void SwapTexture(const char *texturePath);
	void SwapModel(const char *modelPath);

	WorldTimer* GetWorldTimer() const{
		return worldTimer;
	}

	GameObject* CreateGameObject(GameObjectTypes type);
};

