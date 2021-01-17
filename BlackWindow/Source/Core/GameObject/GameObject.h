#pragma once
#include <string>
#include <vector>
#include "Core/Components/ComponentTypes.h"
#include "Math/float4x4.h"
#include "Application.h"
#include"Geometry/AABB.h"
#include"Geometry/OBB.h"
#include "Core/Components/ComponentTransform.h"
#include "Core/Components/ComponentMaterial.h"
#include "Core/Components/ComponentMesh.h"
#include "Core/Components/ComponentMeshRenderer.h"
#include "json/json.h"

class GameObject
{
protected:
    GameObject *parent;
    std::string name;
    unsigned int program;
    int id;
    float4x4 projectionMatrix;
    float4x4 viewMatrix;
    float4x4 modelMatrix;
    std::vector<GameObject *> children;
    std::vector<Component *> components;
  
    ComponentMesh* componentMesh;
    ComponentMaterial * materialComponent;
    ComponentTransform *transformComponent;
    ComponentMeshRenderer *meshRendererComponent;
    AABB globalBoundingBox ;
    AABB selfBoundingBox;
    OBB  selfObb;
public:
    GameObject(GameObject *parent = nullptr, const char *name = "", unsigned int program = 0)
    {
        this->parent = parent;
        this->name = name;
        this->program = program;

        this->id = App->GetLcg()->Int();
        projectionMatrix = float4x4::identity;
        viewMatrix = float4x4::identity;
        modelMatrix = float4x4::identity;

        this->transformComponent = nullptr;
        this->componentMesh = nullptr;

        
        this->materialComponent = nullptr;
    }
    ~GameObject() {}
    virtual void Update();
    virtual void Draw();

    std::vector<GameObject *> GetChildren()
    {
        return children;
    }


    AABB GetSelfBoundingBox() const{
        return selfBoundingBox;
    }
    OBB GetSelfObb() const {
        return selfObb;
    }
    AABB GlobalBoundingBox() const {
        return globalBoundingBox;
    }
    bool HitByRayCast(LineSegment ray);
    bool CheckMeshRayCast(LineSegment ray);

    Component *AddComponent(ComponentTypes type);
    void AddComponent(Component *component);
    void CalculateBox( );
    void AddChildren(GameObject *gameObject);
    void SetProjectionMatrix(const float4x4 &projection);
    void SetViewMatrix(const float4x4 &projection);
    void CalculateMeshBoundingBox();
    void InitializeCheckForRayCast(LineSegment ray);

    unsigned int  GetProgram() {
        return program;
    }
    void CheckForRayCast(LineSegment ray);
    std::vector<Component *> GetComponents()
    {
        return components;
    }

    std::string GetName() const
    {
        return name;
    }

    int GetId() const
    {
        return id;
    }

    void RemoveChild(const GameObject *child);

    void SetParent(GameObject *newParent);
    

    void SetName(const std::string &name)
    {
        this->name = name;
    }

    ComponentTransform *GetTransformComponent() const
    {
        return transformComponent;
    }

    ComponentMesh *GetMeshComponent() const
    {
        return componentMesh;
    }

    float4x4 GetProjectionMatrix()
    {
        return projectionMatrix;
    }

    float4x4 GetViewMatrix()
    {
        return viewMatrix;
    }

    void CalculateModelMatrix();

    bool isChild(GameObject *lookingChild);

    void Save(const char * name);
    void Export(Json::Value &parent);

    float4x4 GetModelMatrix() const
    {
        return modelMatrix;
    }

    void RemoveParent();

    void Clear();

    void CheckDefaultsComponents(Component *component);

    ComponentMaterial * GetMaterialComponent() const
    {
        return materialComponent;
    }

    void ApplyTextureToModel(unsigned int textureId)
    {
        if(materialComponent){
            materialComponent->SetTexture(textureId);
        }
        glUniform1i(glGetUniformLocation(program, "material.diffuse"), 0);
        glUniform1i(glGetUniformLocation(program, "material.specular"), 1);
    }

    ComponentMeshRenderer *GetMeshRendererComponent() const
    {
        return meshRendererComponent;
    }


};
