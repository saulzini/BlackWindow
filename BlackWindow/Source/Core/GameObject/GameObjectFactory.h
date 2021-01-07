#pragma once
#include "Core/GameObject/GameObject.h"
#include "Core/GameObject/GameObjectTypes.h"
#include "Core/Components/ComponentTypes.h"
#include "json/json.h"

namespace GameObjectFactory
{
    
    static GameObject* CreateGameObject(GameObjectTypes type){
        GameObject* gameObject = new GameObject();
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

    static GameObject* CreateGameObjectFromJson(const Json::Value& jRoot, GameObject *parent,unsigned int program){
        GameObject *root =new GameObject(parent,jRoot["name"].asCString(),program);
        // load components
        // create medot in components factory

        return root;
    }

    static GameObject* CreateGameObjectsFromJson(const Json::Value& jRoot, GameObject *parent,unsigned int program){
        GameObject *root =  CreateGameObjectFromJson(jRoot,parent,program);

        // std::queue<Json::Value *> gameObjectsQueu;
        // gameObjectsQueu.push(root);
        // Json::Value *current = nullptr;
        // while( !gameObjectsQueu.empty() ){
        //     current = gameObjectsQueu.front();
        //     gameObjectsQueu.pop();
        
        //     std::vector<GameObject *> currentChildren = current->GetChildren();

        //     if (currentChildren.size()> 0){
        //         for (std::vector<GameObject *>::iterator it = currentChildren.begin(); it != currentChildren.end(); ++it)
        //         {
        //             gameObjectsQueu.push( (GameObject *)*it );
        //         }
        //     }

        //     current->RemoveParent();
        //     current->Clear();
        //     App->editor->consoleWindow->AddLog("Clear %s",current->GetName().c_str());
        // }


        return root;
    }


}