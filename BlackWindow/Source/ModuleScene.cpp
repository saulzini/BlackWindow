#include "ModuleScene.h"
#include "GL/glew.h"
#include "Application.h"
#include "Core/Program/Program.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "Core/Mesh.h"
#include "Math/float4x4.h"
#include <string>
#include "Leaks.h"
#include "../glm/glm.hpp"
#include "Core/Time/WorldTimer.h"
#include "Core/GameObject/GameObjectFactory.h"
#include <Core/Importers/Texture/TextureLoader.h>
ModuleScene::ModuleScene()
{
	program = 0;
	worldTimer = new WorldTimer();

	sky = nullptr;
	programSky = 0;

	root = new GameObject();
}

ModuleScene::~ModuleScene()
{
	
}

bool ModuleScene::Init()
{
	Program programClass;
	GameObject *house = new GameObject(root,"BakerHouse");
	
	// model = new Model(".\\Assets\\BakerHouse\\BakerHouse.fbx");
	root->AddChildren(house);
	program = programClass.CreateProgramFromSource("Default.vert", "Default.frag");
	programSky = programClass.CreateProgramFromSource("DefaultBox.vert", "DefaultBox.frag");
	sky = new Skybox();

	return true;
}

update_status ModuleScene::PreUpdate(float deltaTime)
{
	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update(float deltaTime)
{

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
	

	// model->Draw(program);

	App->scene->sky->Draw();
	worldTimer->RegulateFPS();

	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate(float deltaTime)
{
	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	delete (worldTimer);
	worldTimer = nullptr;
	
	return true;
}

void ModuleScene::SwapTexture(const char *texturePath) 
{
	// std::string textPath(texturePath);
	// std::string directory = textPath.substr(0, textPath.find_last_of('\\'));
	// unsigned int id = TextureLoader::LoadTexture2D(textPath.c_str(),directory.c_str());
	// if (id == 0){
	// 	App->editor->consoleWindow->AddLog("Error in swapping texture");
	// 	return;
	// }
	// model->ApplyTextureToModel(id,textPath.c_str() );
	
}

void ModuleScene::SwapModel(const char *modelPath)
{
	// // Free space of previous model
	// delete (model);
	// model = nullptr;
	// model = new Model(modelPath);

	// App->camera->MoveAccordingNewModelInScene(model->GetDimensions());
}

GameObject* ModuleScene::CreateGameObject(GameObjectTypes type) 
{
	return GameObjectFactory::CreateGameObject(type);
}
