#include "ModuleWorld.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "Core/Mesh.h"
#include "Core/Model.h"
#include "Math/float4x4.h"
#include <string>
#include "Leaks.h"
#include "../glm/glm.hpp"
#include "Core/Time/WorldTimer.h"
ModuleWorld::ModuleWorld()
{
	model = nullptr;
	program = 0;
	worldTimer = new WorldTimer();

	sky = nullptr;
	programSky = 0;
}

ModuleWorld::~ModuleWorld()
{
	
}

bool ModuleWorld::Init()
{


	model = new Model(".\\Assets\\BakerHouse\\BakerHouse.fbx");
	program = App->program->CreateProgramFromSource("Default.vert", "Default.frag");
	programSky = App->program->CreateProgramFromSource("DefaultBox.vert", "DefaultBox.frag");
	sky = new Skybox();

	return true;
}

update_status ModuleWorld::PreUpdate(float deltaTime)
{
	return UPDATE_CONTINUE;
}

update_status ModuleWorld::Update(float deltaTime)
{
	
	//glDisable(GL_DEPTH_TEST);
	//App->world->sky->Draw();

	worldTimer->Update();
	glUseProgram(program);
	float4x4 proj = App->camera->GetProjection();
	float4x4 view = App->camera->GetView();
	float4x4 identityModel = float4x4::identity;
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &identityModel[0][0]); //GL_TRUE transpose the matrix
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &proj[0][0]);


	GLint ks = glGetUniformLocation(program, "ks");
	GLint kd = glGetUniformLocation(program, "kd");
	GLint N = glGetUniformLocation(program, "N");

	GLint light_pos = glGetUniformLocation(program, "light_pos");
	GLint light_color = glGetUniformLocation(program, "light_color");
	GLint viewPos = glGetUniformLocation(program, "viewPos");
	GLint colorAmbient = glGetUniformLocation(program, "colorAmbient");

	glUniform1f(ks, 0.8f);
	glUniform1f(kd, 0.6f);
	glUniform1f(N, 32);

	float3 lightpos = { 1.0f, 0.0f, 0.0f };
	float3 lightcolor = { 1.0f, 1.0f, 1.0f };
	float3 view_Pos = App->camera->cameraPosition;
	float3 color_Ambient = { 1.0f, 1.0f, 1.0f };

	glUniform1i(glGetUniformLocation(program, "texture_diffuse"), 0);
	glUniform3f(light_pos,		   lightpos[0],		 lightpos[1],		lightpos[2]);
	glUniform3f(light_color,	 lightcolor[0],    lightcolor[1],	  lightcolor[2]);
	glUniform3f(viewPos,		   view_Pos[0],		 view_Pos[1],		view_Pos[2]);
	glUniform3f(colorAmbient, color_Ambient[0], color_Ambient[1],  color_Ambient[2]);

	std::cout << glGetError() << std::endl; // returns 0 (no error)
	
	//glDisable(GL_DEPTH_TEST);
	//sky->Draw();
	//glEnable(GL_DEPTH_TEST);
	model->Draw(program);

	//sky->Draw();
	//glDisable(GL_DEPTH_TEST);
	App->world->sky->Draw();
	//glEnable(GL_DEPTH_TEST);
	//(glEnable(GL_DEPTH_TEST);
	worldTimer->RegulateFPS();

	return UPDATE_CONTINUE;
}

update_status ModuleWorld::PostUpdate(float deltaTime)
{
	return UPDATE_CONTINUE;
}

bool ModuleWorld::CleanUp()
{
	delete (model);
	model = nullptr;

	delete (worldTimer);
	worldTimer = nullptr;
	
	return true;
}

void ModuleWorld::SwapTexture(const char *texturePath) 
{
	std::string textPath(texturePath);
	std::string directory = textPath.substr(0, textPath.find_last_of('\\'));
	unsigned int id = TextureLoader::LoadTexture2D(textPath.c_str(),directory.c_str());
	if (id == 0){
		App->editor->consoleWindow->AddLog("Error in swapping texture");
		return;
	}
	model->ApplyTextureToModel(id,textPath.c_str() );
	
}

void ModuleWorld::SwapModel(const char *modelPath)
{
	// Free space of previous model
	delete (model);
	model = nullptr;
	model = new Model(modelPath);

	App->camera->MoveAccordingNewModelInScene(model->GetDimensions());
}
