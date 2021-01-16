#include "ModuleRenderScene.h"
#include "Application.h"
#include "Geometry/Frustum.h"
#include "ModuleInput.h"
#include <Dummy.h>
#include "Geometry/LineSegment.h"
#include "ModuleCamera.h"
ModuleRenderScene::ModuleRenderScene()
{
	worldTimer = new WorldTimer();
}

ModuleRenderScene::~ModuleRenderScene()
{

}

bool ModuleRenderScene::Init()
{
	//frustum = App->camera->GetFrustum();
	return true;
}

update_status ModuleRenderScene::PreUpdate(float deltaTime)
{
	return UPDATE_CONTINUE;
}

update_status ModuleRenderScene::Update(float deltaTime)
{
	if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT)) {
		iPoint newMousePosition = App->input->GetMouseMotion();
		Frustum frustum = App->camera->GetFrustum();
		Triangle* my_ray;
		LineSegment picking = frustum.UnProjectLineSegment(newMousePosition.x, newMousePosition.y);
		//bool hit = my_ray.Intersects()
	}
	return UPDATE_CONTINUE;
}

update_status ModuleRenderScene::PostUpdate(float deltaTime)
{
	return UPDATE_CONTINUE;
}

bool ModuleRenderScene::CleanUp()
{
	delete (worldTimer);
	worldTimer = nullptr;

	return true;
}


