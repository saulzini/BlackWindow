#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
#include <Math/float3.h>
#include "imgui.h"
#include "Math/Quat.h"
#include <vector>
#include "Core/Texture.h"
#include "GL/glew.h"
#include "Core/Components/ComponentMaterial.h"
#include "Core/Components/ComponentMesh.h"
class ComponentMeshRenderer : public Component
{
public:
    ComponentMeshRenderer(GameObject *owner = nullptr, ComponentTypes type = ComponentTypes::MESHRENDERER) : Component(owner, type){
        Setup();
    };

    void Setup();
    void Update() override;
    void OnEditor() override;

    void OnSave(Json::Value& owner) override;
    
private:
    unsigned int vao = 0;
    unsigned int vbo = 0;
    unsigned int ebo = 0;
    size_t indicesSize =0;
};
