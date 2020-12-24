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
// #include "../glm/glm.hpp"
#include "Core/Time/WorldTimer.h"
#include "Core/GameObject/GameObjectFactory.h"
#include <Core/Importers/Texture/TextureLoader.h>
#include "Core/Importers/Model/Model.h"
#include "Core/Components/ComponentTypes.h"
#include "Core/Components/ComponentMesh.h"
#include "MathGeoLibFwd.h"
#include "Math/Quat.h"
ModuleScene::ModuleScene()
{
	program = 0;
	worldTimer = new WorldTimer();

	sky = nullptr;
	programSky = 0;

	root = nullptr;
}

ModuleScene::~ModuleScene()
{
	
}

bool ModuleScene::Init()
{
	Program programClass;
	
	program = programClass.CreateProgramFromSource("Default.vert", "Default.frag");
	programSky = programClass.CreateProgramFromSource("DefaultBox.vert", "DefaultBox.frag");
	
	root = new GameObject(nullptr,"Scene",program);
	sky = new Skybox();
	// Setting gameobject
	ModelImporter::Model *model =new ModelImporter::Model(".\\Assets\\BakerHouse\\BakerHouse.fbx",program); 
	GameObject *house = model->LoadModel();

	root->AddChildren(house);
	return true;
}

update_status ModuleScene::PreUpdate(float deltaTime)
{
	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update(float deltaTime)
{
	//App->scene->sky->Draw();

	worldTimer->Update();

	glUseProgram(program);
	float4x4 proj = App->camera->GetProjection();
	float4x4 view = App->camera->GetView();

	root->SetProjectionMatrix(proj);
	root->SetViewMatrix(view);
	root->Update();

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
	
	///App->scene->sky->Draw();
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
