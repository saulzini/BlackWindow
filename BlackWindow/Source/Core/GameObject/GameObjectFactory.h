#pragma once
#include "Core/GameObject/GameObject.h"
#include "Core/GameObject/GameObjectTypes.h"
#include "Core/Components/ComponentTypes.h"
#include "json/json.h"
#include "Core/Components/ComponentFactory.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "UIWindow/ConsoleWindow.h"
#include "Core/Components/ComponentMeshRenderer.h"

namespace GameObjectFactory
{

    static GameObject *CreateGameObject(GameObjectTypes type, GameObject* parent = nullptr, const char* name = "", unsigned int program = 0)
    {
        GameObject *gameObject = new GameObject(parent, name, program);
        switch (type)
        {
      //  case GameObjectTypes::SCRIPT:
      //      gameObject->AddComponent(ComponentTypes::SCRIPT);
      //      break;
        case GameObjectTypes::CAMERA:
            gameObject->AddComponent(ComponentTypes::TRANSFORM);
            gameObject->AddComponent(ComponentTypes::CAMERA);
            break;
        case GameObjectTypes::LIGHT:
            gameObject->AddComponent(ComponentTypes::TRANSFORM);
            gameObject->AddComponent(ComponentTypes::LIGHT);
            break;
        case GameObjectTypes::DEFAULT:
            gameObject->AddComponent(ComponentTypes::TRANSFORM);
            break;
        default:
            gameObject->AddComponent(ComponentTypes::NONE);
            break;
        }
        return gameObject;
    }

    static GameObject *CreateGameObjectFromJson(const Json::Value &jRoot, GameObject *parent)
    {

        Json::Value nodeRoot;
        std::ifstream file( jRoot["id"].asCString() ) ;
        if(file.fail()){
            App->editor->consoleWindow->AddLog( "%s not exits" , jRoot["id"].asCString());
            return nullptr;
        }
        file >> nodeRoot;
        unsigned int aux;
        Program programClass;
        aux = programClass.CreateProgramFromSource("Default.vert", "Default.frag");
        glUseProgram(aux);

		App->editor->consoleWindow->AddLog("Loading %s",nodeRoot["name"].asCString() );
        GameObject *root = new GameObject(parent, nodeRoot["name"].asCString(), aux);
        // load components
        for (unsigned int index = 0; index < nodeRoot["components"].size(); index++)
        {
            Component *component = ComponentFactory::CreateComponentFromJson(nodeRoot["components"][index],root);
            root->AddComponent(component);

        }
        //Load children
        for (unsigned int index = 0; index < nodeRoot["children"].size(); index++)
        {
            root->AddChildren( CreateGameObjectFromJson(nodeRoot["children"][index],root) );
        }
        root->CalculateMeshBoundingBox();

        return root;
    }

    

} // namespace GameObjectFactory