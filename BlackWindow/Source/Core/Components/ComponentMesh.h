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

    void SetMesh(Mesh &mesh)
    {
        this->mesh = mesh;
    }
    
    void Update() override
    {
        if ( active == false) {
            return;
        }

        mesh.Draw(shader);
        // for (unsigned int i = 0; i < meshes.size(); i++)
        // {
        //     meshes[i].Draw(shader);
        // }
    }

    void OnSave(Json::Value& parent) override
    {
        Json::Value meshJson;
        mesh.Save(meshJson);
        parent["mesh"].append(meshJson);
    }
};

