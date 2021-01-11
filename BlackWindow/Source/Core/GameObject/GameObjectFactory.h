#pragma once
#include "Core/GameObject/GameObject.h"
#include "Core/GameObject/GameObjectTypes.h"
#include "Core/Components/ComponentTypes.h"
#include "json/json.h"
#include "Core/Components/ComponentFactory.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "UIWindow/ConsoleWindow.h"
namespace GameObjectFactory
{

    static GameObject *CreateGameObject(GameObjectTypes type)
    {
        GameObject *gameObject = new GameObject();
        switch (type)
        {
        case GameObjectTypes::SCRIPT:
            gameObject->AddComponent(ComponentTypes::SCRIPT);
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

    static GameObject *CreateGameObjectFromJson(const Json::Value &jRoot, GameObject *parent, unsigned int program)
    {
		App->editor->consoleWindow->AddLog("Loading %s",jRoot["name"].asCString() );
        GameObject *root = new GameObject(parent, jRoot["name"].asCString(), program);
        // load components
        for (unsigned int index = 0; index < jRoot["components"].size(); index++)
        {
            root->AddComponent(ComponentFactory::CreateComponentFromJson(jRoot["components"][index],root));
        }
        //Load children
        for (unsigned int index = 0; index < jRoot["children"].size(); index++)
        {
            root->AddChildren( CreateGameObjectFromJson(jRoot["children"][index],root,program) );
        }

        return root;
    }

    

} // namespace GameObjectFactory