#include "ModuleScene.h"
#include "GL/glew.h"
#include "Application.h"
#include "Core/Program/Program.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleDebugDraw.h"
#include "Application.h"
#include "Math/float4x4.h"
#include <string>
#include "Leaks.h"
#include "Core/Time/WorldTimer.h"
#include "Core/GameObject/GameObjectFactory.h"
#include "Core/Importers/Texture/TextureLoader.h"
#include "Core/Components/ComponentFactory.h"
#include "Core/Importers/Model/Model.h"
#include "Core/Components/ComponentTypes.h"
#include "Core/Components/ComponentLight.h"
#include "Core/Components/ComponentMesh.h"
#include "MathGeoLibFwd.h"
#include "Math/Quat.h"
#include <queue>
#include "ModuleEditor.h"
#include "UIWindow/ConsoleWindow.h"
#include "Core/SceneFileManager/SceneFileManager.h"
#include "ImGuizmo.h"

ModuleScene::ModuleScene()
{
	program = 0;
	worldTimer = new WorldTimer();

	sky = nullptr;
	programSky = 0;

	root = nullptr;
	selected = nullptr;
	Light = nullptr;
	Camera = nullptr;
}

ModuleScene::~ModuleScene()
{
	
}

bool ModuleScene::Init()
{
	Program programClass;
	
	program = programClass.CreateProgramFromSource("Default.vert", "Default.frag");
	program2 = programClass.CreateProgramFromSource("Default.vert", "Default.frag");
	programSky = programClass.CreateProgramFromSource("DefaultBox.vert", "DefaultBox.frag");
	
	root = new GameObject(nullptr,"Scene",program);
	sky = new Skybox();
	// Setting gameobject
	Light = GameObjectFactory::CreateGameObject(GameObjectTypes::LIGHT, root, "Light", program2);
	root->AddChildren(Light);
	Light->GetTransformComponent()->SetPosition(float3 (0.0f, 1.0f,0.0f));

	// Camera = GameObjectFactory::CreateGameObject(GameObjectTypes::CAMERA, root, "Camera", program);
	// root->AddChildren(Camera);
	//Camera->GetTransformComponent()->SetPosition(float3(0.0f, 1.0f, 0.0f));
	
	//house->GetTransformComponent()->SetScale(float3(0.01f, 0.01f, 0.01f));

	//Light->GetTransformComponent()->SetPosition(float3(0.0f, 5.0f, 3.0f));

	// SceneFileManager::LoadFromFile("scene.blackwindow");
	SceneFileManager::LoadScene();

	//Light->Update();
	
	return true;
}

update_status ModuleScene::PreUpdate(float deltaTime)
{
	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update(float deltaTime)
{

	DrawGuizmo();
	App->scene->sky->Draw();

	worldTimer->Update();

	float4x4 proj = App->camera->GetProjection();
	float4x4 view = App->camera->GetView();

	root->SetProjectionMatrix(proj);
	root->SetViewMatrix(view);
	root->CalculateBox();
	root->Update();


	//GLint N = glGetUniformLocation(program, "material.shininess");

	////GLint light_pos = glGetUniformLocation(program, "light_pos");
	//GLint light_ambient = glGetUniformLocation(program, "light.ambient");
	//GLint light_diffuse = glGetUniformLocation(program, "light.diffuse");
	//GLint light_specular = glGetUniformLocation(program, "light.specular");
	//GLint viewPos = glGetUniformLocation(program, "viewPos");
	//GLint colorAmbient = glGetUniformLocation(program, "colorAmbient");



	////glUniform1f(N, 32);

	//float3 lightpos(0.0f,0.0f,0.0f);  
	//if (Light){
	//	lightpos = Light->GetTransformComponent()->GetPosition();
	//}
	//float3 lightambient = { 0.2f, 0.2f, 0.2f };
	//float3 lightdiffuse = { 0.5f, 0.5f, 0.5f };
	//float3 lightspecular = { 1.0f, 1.0f, 1.0f };
	//float3 view_Pos = App->camera->cameraPosition;
	//float3 color_Ambient = { 1.0f, 1.0f, 1.0f };

	///*glUniform1i(glGetUniformLocation(program, "material.diffuse"), 0);
	//glUniform1i(glGetUniformLocation(program, "material.specular"), 1);*/

	////glUniform3f(light_pos,			 lightpos[0],			 lightpos[1],				lightpos[2]);
	//glUniform3f(light_ambient,   lightambient[0],		 lightambient[1],			lightambient[2]);
	//glUniform3f(light_diffuse,	 lightdiffuse[0],		 lightdiffuse[1],			lightdiffuse[2]);
	//glUniform3f(light_specular, lightspecular[0],		lightspecular[1],		   lightspecular[2]);
	//glUniform3f(viewPos,			 view_Pos[0],			 view_Pos[1],				view_Pos[2]);
	//glUniform3f(colorAmbient,	color_Ambient[0],		color_Ambient[1],		   color_Ambient[2]);

	std::cout << glGetError() << std::endl; // returns 0 (no error)
	
	// App->scene->sky->Draw();

	int w = 0;
	int h = 0;
	App->draw->Draw(App->camera, w, h);
	dd::xzSquareGrid(-200, 200, 0.0f, 1.0f, dd::colors::Gray);
	dd::axisTriad(float4x4::identity, 0.5f, 5.1f);
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

void ModuleScene::DrawGuizmo() {
	//TODO :: NOT ENOUGHT TIME 
	/* ImGuizmo::BeginFrame();
	ImGuizmo::Enable(true);
	ImVec2 pos = ImGui::GetWindowPos();
	ImGuizmo::SetRect(App->camera->cameraPosition.x, App->camera->cameraPosition.y, App->window->GetWidth(), App->window->GetHeight());
	float cameraView[16] =
	{ 1.f, 0.f, 0.f, 0.f,
	  0.f, 1.f, 0.f, 0.f,
	  0.f, 0.f, 1.f, 0.f,
	  0.f, 0.f, 0.f, 1.f };
	float cameraProjection[16];
	static const float identityMatrix[16] =
	{ 1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f };
	if (GetSelected() == nullptr) {
		return;
	}
	float4x4 model = GetSelected()->GetModelMatrix();
	
	float4x4 delta;
	float4x4 view = App->camera->GetView();
	view.Transpose();
	ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
	ImGuizmo::SetDrawlist();
	ImGuizmo::Manipulate((const float*)&view, (const float*)&App->camera->GetProjection() , ImGuizmo::TRANSLATE , ImGuizmo::WORLD, (float*)&model, (float*)&delta, NULL);*/ 
}

void ModuleScene::AddTexture(const char *texturePath) 
{
	GameObject *selected = GetSelected();

	if (selected == nullptr){
		App->editor->consoleWindow->AddLog("Select a gameobject to drag a texture");
		return;
	}
	
	std::string textPath(texturePath);
	std::string directory = textPath.substr(0, textPath.find_last_of('\\'));
	unsigned int id = TextureImporter::TextureLoader::GetTextureIdByPath(textPath.c_str(),directory.c_str());
	if (id == 0){
		App->editor->consoleWindow->AddLog("Error in adding texture");
		return;
	}

	selected->ApplyTextureToModel(id);
}

void ModuleScene::AddModel(const char *modelPath)
{
	//Creating program for the new GameObject
	unsigned int aux;
	Program programClass;
	aux = programClass.CreateProgramFromSource("Default.vert", "Default.frag");
	glUseProgram(aux);
	ModelImporter::Model *model =new ModelImporter::Model(modelPath, aux);
	GameObject *gameObject = model->LoadModel();
	App->camera->MoveAccordingNewModelInScene(gameObject->GlobalBoundingBox().maxPoint);
	delete model;
	root->AddChildren(gameObject);
}

GameObject* ModuleScene::CreateGameObject(GameObjectTypes type) 
{
	return GameObjectFactory::CreateGameObject(type);
}


 void ModuleScene::SaveScene(const char *name){
	root->Save(name);
}

void ModuleScene::Load(const Json::Value& jRoot)
{
	// delete from scene 
    DeleteGameObjects();

	// load new gameobjects
	LoadFromJson(jRoot);
}

void ModuleScene::LoadFromJson(const Json::Value& jRoot) 
{
	// ignoring the first level because it is the scene
	for (Json::Value::ArrayIndex i = 0; i != jRoot["children"].size(); i++){
		GameObject *child = GameObjectFactory::CreateGameObjectFromJson(jRoot["children"][i],root);
		root->AddChildren(child);
	}
}


void ModuleScene::DeleteGameObjects() 
{
	if (root == nullptr){
		return;
	}

	std::queue<GameObject *> gameObjectsQueu;
	gameObjectsQueu.push(root);
    GameObject *current = nullptr;
    while( !gameObjectsQueu.empty() ){
        current = gameObjectsQueu.front();
        gameObjectsQueu.pop();
      
        std::vector<GameObject *> currentChildren = current->GetChildren();

        if (currentChildren.size()> 0){
            for (std::vector<GameObject *>::iterator it = currentChildren.begin(); it != currentChildren.end(); ++it)
            {
                gameObjectsQueu.push( (GameObject *)*it );
            }
        }

		current->RemoveParent();
		current->Clear();
		App->editor->consoleWindow->AddLog("Clear %s",current->GetName().c_str());
    }
}