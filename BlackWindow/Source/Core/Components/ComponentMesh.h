#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
#include "Core/Mesh.h"
class ComponentMesh : public Component
{ 

public:
    Mesh *mesh;
    unsigned int shader;
public:
    ComponentMesh(GameObject *owner,ComponentTypes type = ComponentTypes::MESH, unsigned int shader = 0) : Component(owner,type){
        this->shader = shader;
        mesh = nullptr;
    }


    void SetShader(unsigned int shader){
        this->shader = shader;
    }

    void SetMesh(Mesh *mesh){
        this->mesh = mesh;
    }
    
    void Update() override{
        if (mesh == nullptr || active == false) {
            return;
        }

        mesh->Draw(shader);
        // for (unsigned int i = 0; i < meshes.size(); i++)
        // {
        //     meshes[i].Draw(shader);
        // }
    }
};

