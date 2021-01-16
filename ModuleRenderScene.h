#pragma once
#include "Module.h"
#include "Core/Time/WorldTimer.h"
#include "Core/GameObject/GameObjectTypes.h"
#include "json/json.h"
#include "Geometry/Frustum.h"
class GameObject;

class ModuleRenderScene : public Module
{

private:

	WorldTimer* worldTimer;
	Frustum* frustum;

public:
	ModuleRenderScene();
	~ModuleRenderScene();

	bool Init() override;
	update_status PreUpdate(float deltaTime) override;
	update_status Update(float deltaTime) override;
	update_status PostUpdate(float deltaTime) override;
	bool CleanUp() override;

	
	WorldTimer* GetWorldTimer() const {
		return worldTimer;
	}

};