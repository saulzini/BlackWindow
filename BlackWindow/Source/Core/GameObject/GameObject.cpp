#include "GameObject.h"
#include "Geometry/Triangle.h"
#include "Core/Components/ComponentFactory.h"
#include "Core/GameObject/GameObjectTypes.h"
#include "Math/float3x3.h"
#include "GL/glew.h"
#include "Geometry/AABB.h"
#include "Math/Quat.h"
#include "Geometry/LineSegment.h"
#include <queue> // std::queue
#include <stack> // std::stack
#include "debugdraw.h"
#include "Core/Vertex.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "UIWindow/ConsoleWindow.h"
#include "Core/SceneFileManager/SceneFileManager.h"
#include <string>

Component *GameObject::AddComponent(ComponentTypes type)
{
    Component *component = ComponentFactory::CreateComponent(this, type);
    components.push_back(component);

    CheckDefaultsComponents(component);
    return component;
}

void GameObject::AddComponent(Component *component)
{
    CheckDefaultsComponents(component);
    components.push_back(component);
}

void GameObject::AddChildren(GameObject *gameObject)
{
    // adding children
    children.push_back(gameObject);

    // adding parent of child
    gameObject->parent = this;
}

void GameObject::SetProjectionMatrix(const float4x4 &projection)
{
    projectionMatrix = projection;
}

void GameObject::SetViewMatrix(const float4x4 &view)
{
    viewMatrix = view;
}

void GameObject::RemoveChild(const GameObject *child)
{
    if (children.empty())
    {
        return;
    }
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

void GameObject::SetParent(GameObject *newParent)
{
    // parent position
    float4x4 oldParentMatrix = float4x4::identity;
    float4x4 newParentMatrix = newParent->GetModelMatrix();

    // delete from parent
    if (parent != nullptr)
    {
        oldParentMatrix = parent->GetModelMatrix();
        parent->RemoveChild(this);
    }
    // asssigning new parent
    parent = newParent;
    parent->AddChildren(this);

    float3 translateOld(0.0f);
    Quat rotationOld(0.0f, 0.0f, 0.0f, 0.0f);
    float3 scaleOld(0.0f);

    float3 translateNew(0.0f);
    Quat rotationNew(0.0f, 0.0f, 0.0f, 0.0f);
    float3 scaleNew(0.0f);

    oldParentMatrix.Decompose(translateOld, rotationOld, scaleOld);
    newParentMatrix.Decompose(translateNew, rotationNew, scaleNew);

    Quat rotationCurrent = transformComponent->GetRotationQuat();

    float3 newRotation = float3(rotationOld.x + rotationCurrent.x - rotationNew.x,
                                rotationOld.y + rotationCurrent.y - rotationNew.y,
                                rotationOld.z + rotationCurrent.z - rotationNew.z);

    if (transformComponent)
    {
        transformComponent->SetPosition((translateOld + transformComponent->GetPosition()) - translateNew);
        transformComponent->SetScale((scaleOld + transformComponent->GetScale()) - scaleNew);
        transformComponent->SetRotation(newRotation);
    }
    CalculateModelMatrix();
}

void GameObject::CalculateModelMatrix()
{
    modelMatrix = float4x4::identity;
    if (transformComponent)
    {
        modelMatrix = float4x4::FromTRS(
            transformComponent->GetPosition(),
            transformComponent->GetRotationQuat(),
            transformComponent->GetScale());
    }
}

bool GameObject::isChild(GameObject *lookingChild)
{
    if (children.size() <= 0)
    {
        return false;
    }

    std::queue<GameObject *> searchQueue;
    searchQueue.push(this);
    GameObject *current = nullptr;
    while (!searchQueue.empty())
    {
        current = searchQueue.front();
        searchQueue.pop();
        if (current == lookingChild)
        {
            return true;
        }

        std::vector<GameObject *> currentChildren = current->GetChildren();

        if (currentChildren.size() > 0)
        {
            for (std::vector<GameObject *>::iterator it = currentChildren.begin(); it != currentChildren.end(); ++it)
            {
                searchQueue.push(*it);
            }
        }
    }

    return false;
}

void GameObject::CalculateMeshBoundingBox()
{
    if (componentMesh)
    {
        selfBoundingBox.SetNegativeInfinity();
        std::vector<float3> verticesPosition = componentMesh->GetVerticesPosition();
        selfBoundingBox.Enclose(&verticesPosition[0], static_cast<int>(verticesPosition.size()));
    }
}

void GameObject::CalculateBox()
{
    std::vector<GameObject *> currentChildren = this->GetChildren();
    for (std::vector<GameObject *>::iterator it = currentChildren.begin(); it != currentChildren.end(); ++it)
    {
        (*it)->CalculateBox();
    }

    if (componentMesh != nullptr)
    {
        globalBoundingBox.SetNegativeInfinity();
        selfObb = selfBoundingBox.Transform(modelMatrix);
        globalBoundingBox.Enclose(selfObb);
        dd::aabb(globalBoundingBox.minPoint, globalBoundingBox.maxPoint, dd::colors::Red);
    }
}


void GameObject::InitializeCheckForRayCast(LineSegment ray) 
{
    for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
    {
        (*it)->CheckForRayCast(ray);
    }
}

void GameObject::CheckForRayCast(LineSegment ray)
{
    std::vector<GameObject *> results;

    std::queue<GameObject *> searchQueue;
    searchQueue.push(this);
    GameObject *current = nullptr;
    while (!searchQueue.empty())
    {
        current = searchQueue.front();
        searchQueue.pop();

        std::vector<GameObject *> currentChildren = current->GetChildren();

        if (currentChildren.size() > 0)
        {
            for (std::vector<GameObject *>::iterator it = currentChildren.begin(); it != currentChildren.end(); ++it)
            {
                searchQueue.push(*it);
            }
        }

        if (current-> GetMeshComponent() ){

            bool hit = current->HitByRayCast(ray);
            if (hit)
            {
                results.push_back(current);
                App->editor->consoleWindow->AddLog("Listed : %s", current->GetName().c_str());
            }
        }
    }


    //results;
}

bool GameObject::HitByRayCast(LineSegment ray)
{
    // checking against aabb
    if ( ray.Intersects(globalBoundingBox) )
    {
        // check agains mesh
        return CheckMeshRayCast(ray);
    }
    return false;
}

bool GameObject::CheckMeshRayCast(LineSegment ray)
{

    if (componentMesh == nullptr){
        return false;
    }
    // test all triangles
    LineSegment localLineSegment(ray);
    localLineSegment.Transform(GetModelMatrix().Inverted());

    
    Triangle meshTriangle;
    std::vector<Vertex> vertices =  componentMesh->GetVertices();
    std::vector<unsigned int> indices =  componentMesh->GetIndices();

    // TODO::Not enough time
    // for (unsigned int i = 0; i < indices.size();)
    // {
    //     meshTriangle.a = vertices[indices[i++]].GetPosition();
    //     meshTriangle.b = vertices[indices[i++]].GetPosition();
    //     meshTriangle.c = vertices[indices[i++]].GetPosition();

    //     float distance;
    //     float3 hitPoint;
    //     if (localLineSegment.Intersects(meshTriangle, &distance, &hitPoint))
    //     {
    //         return true;
    //     }
    // }
    return false;
}
void GameObject::Save(const char *name)
{
    Json::Value jsonRoot;
    jsonRoot["name"] = this->GetName();
    jsonRoot["children"] = Json::arrayValue;
    std::vector<GameObject *> currentChildren = this->GetChildren();

    for (std::vector<GameObject *>::iterator it = currentChildren.begin(); it != currentChildren.end(); ++it)
    {
        (*it)->Export(jsonRoot);
    }

    SceneFileManager::ExportFile(name, jsonRoot);
}

void GameObject::Export(Json::Value &parent)
{
    Json::Value currentJson;
    currentJson["name"] = GetName();
    currentJson["id"] = GetId();

    // components
    currentJson["components"] = Json::arrayValue;
    std::vector<Component *> currentComponents = GetComponents();

    for (std::vector<Component *>::iterator it = currentComponents.begin(); it != currentComponents.end(); ++it)
    {
        (*it)->OnSave(currentJson);
    }

    // children
    currentJson["children"] = Json::arrayValue;
    std::vector<GameObject *> currentChildren = GetChildren();

    for (std::vector<GameObject *>::iterator it = currentChildren.begin(); it != currentChildren.end(); ++it)
    {
        (*it)->Export(currentJson);
    }

    // Saving to file
    SceneFileManager::ExportFile(std::to_string(GetId()).c_str(), currentJson);

    // Addint to parent the id for the generated file
    Json::Value currentJsonGame;
    currentJsonGame["id"] = std::to_string(GetId());
    parent["children"].append(currentJsonGame);
}

void GameObject::RemoveParent()
{
    if (parent == nullptr)
    {
        return;
    }
    // removing from parent
    if (parent != nullptr)
    {
        parent->RemoveChild(this);
    }
    // removing relation
    parent = nullptr;
}

void GameObject::Clear()
{
    // Clear components
    if (!components.empty())
    {
        for (std::vector<Component *>::iterator it = components.begin(); it != components.end(); ++it)
        {
            (*it)->Clear();
        }
        components.clear();
    }
}

void GameObject::CheckDefaultsComponents(Component *component)
{

    switch (component->GetType())
    {
    case ComponentTypes::TRANSFORM:
        transformComponent = static_cast<ComponentTransform *>(component);
        break;

    case ComponentTypes::MATERIAL:
        materialComponent = static_cast<ComponentMaterial *>(component);
        break;

    case ComponentTypes::MESH:
        componentMesh = static_cast<ComponentMesh *>(component);
        break;

    case ComponentTypes::MESHRENDERER:
        meshRendererComponent = static_cast<ComponentMeshRenderer *>(component);
        break;
    }
}

void GameObject::Update()
{
    Draw();

    for (std::vector<Component *>::iterator it = components.begin(); it != components.end(); ++it)
    {
        (*it)->Update();
    }

    for (std::vector<GameObject *>::iterator it = children.begin(); it != children.end(); ++it)
    {
        (*it)->SetProjectionMatrix(projectionMatrix);
        (*it)->SetViewMatrix(viewMatrix);
        (*it)->Update();
    }
}

void GameObject::Draw()
{
    CalculateModelMatrix();
    if (parent)
    {
        modelMatrix = parent->GetModelMatrix() * modelMatrix;
    }

    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &modelMatrix[0][0]); //GL_TRUE transpose the matrix
    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &viewMatrix[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &projectionMatrix[0][0]);
}
