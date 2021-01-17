#pragma once
#include "Module.h"
#include "Core/Time/WorldTimer.h"
#include "Core/Skybox.h"
#include "Core/GameObject/GameObjectTypes.h"
#include "json/json.h"
class GameObject;

class ModuleScene : public Module
{

private:
	unsigned int program;
	WorldTimer *worldTimer;
	GameObject *root;
	GameObject *selected;
	GameObject *Light;
	GameObject *Camera;
public:
	ModuleScene();
	~ModuleScene();
	unsigned int programSky;
	bool Init() override;
	update_status PreUpdate(float deltaTime) override;
	update_status Update(float deltaTime) override;
	update_status PostUpdate(float deltaTime) override;
	bool CleanUp() override;

	Skybox *GetSkyBox() const
	{
		return sky;
	}

	GameObject *GetRoot() const
	{
		return root;
	}

	GameObject *GetSelected() const
	{
		if (selected == root)
		{
			return nullptr;
		}
		return selected;
	}

	void SetSelected(GameObject *gameObject)
	{
		if (selected != gameObject)
		{
			selected = gameObject;
		}
	}

	void SaveScene(const char *name);
	void PickObject();
	void Load(const Json::Value &parent);
	void DeleteGameObjects();
	void DrawGuizmo();
	// For testing
	Skybox *sky;
	void AddTexture(const char *texturePath);
	void AddModel(const char *modelPath);

	WorldTimer *GetWorldTimer() const
	{
		return worldTimer;
	}

	GameObject *CreateGameObject(GameObjectTypes type);

	void LoadFromJson(const Json::Value &jRoot);
};