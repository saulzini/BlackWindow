#include "ModuleWorld.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "Mesh.h"
#include "Model.h"
#include "MathGeoLib-master/src/Math/float4x4.h"

ModuleWorld::ModuleWorld()
{
	program = 0;
}

ModuleWorld::~ModuleWorld() 
{
	delete(model);
	model = nullptr;
}

bool ModuleWorld::Init()
{
	model = new Model("Assets/BakerHouse/BakerHouse.fbx");
	SwapModel("Assets/Lampara/lamp.fbx");
	//model = new Model("Assets/images/images/lamp.fbx");
	//model = new Model("Assets/Lampara/lamp.fbx");
	program = App->program->CreateProgramFromSource("Default.vert", "Default.frag");
	return true;
}

update_status ModuleWorld::PreUpdate(float deltaTime)
{
	return UPDATE_CONTINUE;
}

update_status ModuleWorld::Update(float deltaTime)
{
	glUseProgram(program);
	float4x4 proj = App->camera->GetProjection();
	float4x4 view = App->camera->GetView();
	float4x4 identityModel = float4x4::identity;
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &identityModel[0][0]); //GL_TRUE transpose the matrix
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &proj[0][0]);
	
	model->Draw(program);
	//enemy1->Draw(program);


	return UPDATE_CONTINUE;
}

update_status ModuleWorld::PostUpdate(float deltaTime)
{
	return UPDATE_CONTINUE;
}

bool ModuleWorld::CleanUp()
{
	return true;
}

void ModuleWorld::SwapModel(const char *modelPath) 
{
	// Free space of previous model
	delete(model);
	model = nullptr;
	model = new Model(modelPath);
	
	App->camera->MoveAccordingNewModelInScene( model->GetDimensions() );
}

