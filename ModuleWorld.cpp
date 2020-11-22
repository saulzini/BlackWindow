#include "ModuleWorld.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "Mesh.h"
#include "Model.h"
#include "MathGeoLib-master/src/Math/float4x4.h"

ModuleWorld::ModuleWorld()
{
	house = nullptr;
	enemy1 = nullptr;
	program = 0;
}

bool ModuleWorld::Init()
{
	 //house = new Model("BakerHouse.fbx");
	// house = new Model("BakerHouse/BakerHouse.fbx");
	house = new Model("Cubo/cubo.fbx");
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
	float4x4 model = float4x4::identity;
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &model[0][0]); //GL_TRUE transpose the matrix
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &proj[0][0]);
	
	house->Draw(program);
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

