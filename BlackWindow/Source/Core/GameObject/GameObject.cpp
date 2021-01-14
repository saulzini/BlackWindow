#include "GameObject.h"
#include "Core/Components/ComponentFactory.h"
#include "Core/GameObject/GameObjectTypes.h"
#include "GL/glew.h"
#include "Math/Quat.h"
#include <queue> // std::queue
#include <stack> // std::stack
#include "Application.h"
#include "ModuleEditor.h"
#include "UIWindow/ConsoleWindow.h"
#include "Core/SceneFileManager/SceneFileManager.h"
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
                searchQueue.push((GameObject *)*it);
            }
        }
    }

    return false;
}

void GameObject::Save()
{
    Json::Value jsonRoot;
    jsonRoot["name"] = this->GetName();
    jsonRoot["children"] = Json::arrayValue;
    std::vector<GameObject *> currentChildren = this->GetChildren();

    for (std::vector<GameObject *>::iterator it = currentChildren.begin(); it != currentChildren.end(); ++it)
    {
        ((GameObject *)*it)->Export(jsonRoot);
    }

    SceneFileManager::ExportFile("scene.blackwindow", jsonRoot);
}

void GameObject::Export(Json::Value &parent)
{
    Json::Value currentJson;
    currentJson["name"] = this->GetName();
    currentJson["id"] = this->GetId();

    // components
    currentJson["components"] = Json::arrayValue;
    std::vector<Component *> currentComponents = this->GetComponents();

    for (std::vector<Component *>::iterator it = currentComponents.begin(); it != currentComponents.end(); ++it)
    {
        ((Component *)*it)->OnSave(currentJson);
    }

    // children
    currentJson["children"] = Json::arrayValue;
    std::vector<GameObject *> currentChildren = this->GetChildren();

    for (std::vector<GameObject *>::iterator it = currentChildren.begin(); it != currentChildren.end(); ++it)
    {
        ((GameObject *)*it)->Export(currentJson);
    }

    parent["children"].append(currentJson);
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
    if (component->GetType() == ComponentTypes::TRANSFORM)
    {
        transformComponent = static_cast<ComponentTransform *>(component);
    }

    if (component->GetType() == ComponentTypes::MATERIAL)
    {
        materialComponents.push_back(static_cast<ComponentMaterial *>(component));
    }

    if (component->GetType() == ComponentTypes::MESH)
    {
        meshComponent = static_cast<ComponentMesh *>(component);
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

    

    float4x4 calculatedModel = float4x4::identity;

    if (transformComponent)
    {
        calculatedModel = float4x4::FromTRS(transformComponent->GetPosition(), transformComponent->GetRotationQuat(), transformComponent->GetScale());
    }

    modelMatrix = calculatedModel;
    // CalculateModelMatrix();
    if (parent)
    {
        modelMatrix = parent->GetModelMatrix() * modelMatrix;
    }

    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &modelMatrix[0][0]); //GL_TRUE transpose the matrix
    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &viewMatrix[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &projectionMatrix[0][0]);
}

