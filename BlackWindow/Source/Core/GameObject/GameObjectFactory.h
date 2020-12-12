#pragma once
#include "Core/GameObject/GameObject.h"
#include "Core/GameObject/GameObjectTypes.h"
#include "Core/Components/ComponentTypes.h"
namespace GameObjectFactory
{
    
    static GameObject* CreateGameObject(GameObjectTypes type){
        GameObject* gameObject = new GameObject();
        switch (type)
        {
            case GameObjectTypes::SCRIPT:
                gameObject->AddComponent(ComponentTypes::SCRIPT);
                break;
            default:
                gameObject->AddComponent(ComponentTypes::TRANSFORM); 
                break;
        }   
        return gameObject;
    }
}