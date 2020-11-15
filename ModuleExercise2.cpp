#include "ModuleExercise2.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "Texture2D.h"
#include "ModuleCamera.h"
#include "Mesh.h"
#include "Model.h"
#include "MathGeoLib-master/src/Math/float4x4.h"

ModuleExercise2::ModuleExercise2()
{
	house = nullptr;
	program = 0;
}

bool ModuleExercise2::Init()
{
	house = new Model("BakerHouse.fbx");
	program = App->program->CreateProgramFromSource("HelloWorld.vert", "HelloWorld.frag");
	return true;
}

update_status ModuleExercise2::PreUpdate(float deltaTime)
{
	return UPDATE_CONTINUE;
}

update_status ModuleExercise2::Update(float deltaTime)
{
	glUseProgram(program);
	float4x4 proj = App->camera->GetProjection();
	float4x4 view = App->camera->GetView();
	float4x4 model = float4x4::identity;

	//float4x4 model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f), float4x4::RotateZ(pi / 4.0f), float3(2.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &model[0][0]); //GL_TRUE transpose the matrix
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &proj[0][0]);
	house->Draw(program);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	//glUniform1i(glGetUniformLocation(program, "mytexture"), 0);

	//glDrawArrays(GL_TRIANGLES, 0, 3);
	return UPDATE_CONTINUE;
}

update_status ModuleExercise2::PostUpdate(float deltaTime)
{
	return UPDATE_CONTINUE;
}

bool ModuleExercise2::CleanUp()
{
	return true;
}

