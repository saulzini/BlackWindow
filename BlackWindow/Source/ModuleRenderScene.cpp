#include "ModuleRenderScene.h"
#include "ModuleScene.h"
#include "Geometry/Plane.h"
#include "ModuleWindow.h"
#include "Geometry/Frustum.h"
#include "Application.h"
#include "Core/GameObject/GameObject.h"
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
	frustum = App->camera->GetFrustum();
	return true;
}

update_status ModuleRenderScene::PreUpdate(float deltaTime)
{
	return UPDATE_CONTINUE;
}

update_status ModuleRenderScene::Update(float deltaTime)
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
		GameObject* root = App->scene->GetRoot();
		if (root == nullptr) {
			return UPDATE_CONTINUE;
		}

		iPoint newMousePosition = App->input->GetMousePosition();
		Triangle* my_ray;
		float width = (float) App->window->GetWidth();
		float height = (float) App->window->GetHeight();
		float normalized_x = -(1.0f - (float(newMousePosition.x) * 2.0f) / width);
		float normalized_y = 1.0f - (float(newMousePosition.y) * 2.0f) / height;
		LineSegment picking = App->camera->GetFrustum().UnProjectLineSegment(normalized_x, normalized_y);
		root->CheckForRayCast(picking);

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


