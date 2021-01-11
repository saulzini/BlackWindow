#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
#include "Core/Mesh.h"
class ComponentMesh : public Component
{ 

public:
    Mesh mesh;
    unsigned int shader;
public:
    ComponentMesh(GameObject *owner = nullptr,ComponentTypes type = ComponentTypes::MESH, unsigned int shader = 0) : Component(owner,type)
    {
        this->shader = shader;
    }


    void SetShader(unsigned int shader)
    {
        this->shader = shader;
    }

    void SetMesh(const Mesh &mesh)
    {
        this->mesh = mesh;
    }
    
    void Update() override
    {
        if ( active == false) {
            return;
        }

        mesh.Draw(shader);
    }

    void OnSave(Json::Value& owner) override
    {
        Json::Value meshJson;
        meshJson["type"] = static_cast<int>(ComponentTypes::MESH);
        mesh.Save(meshJson);
        owner["components"].append(meshJson);
    }

    void Clear() override
    {   
        mesh.Clear();
    }

    void OnLoad(const Json::Value& componentJson) override
    {
        mesh.LoadFromJson(componentJson);
    }

};

