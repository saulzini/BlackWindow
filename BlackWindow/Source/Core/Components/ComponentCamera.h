#pragma once
#include "Geometry/Frustum.h"
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
#include <Math/float3.h>
#include "imgui.h"
#include "debugdraw.h"
#include "Application.h"
#include "Math/Quat.h"

//#include "Core/GameObject/GameObject.h"

class ComponentCamera : public Component
{
public:
    ComponentCamera(GameObject *owner = nullptr, ComponentTypes type = ComponentTypes::CAMERA) : Component(owner, type)
    {
        transformComponent = owner->GetTransformComponent();
        if (transformComponent == nullptr)
        {
            return;
        }
        
        //Setting frustum
        frustum.SetPos(float3::zero);
        frustum.SetFront(float3::unitZ);
        frustum.SetUp(float3::unitY);
        frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
        frustum.SetViewPlaneDistances(0.1f, 10.0f);
        frustum.SetHorizontalFovAndAspectRatio(50.0f, 1.3f);

    };

    void OnEditor() override
    {

        if (ImGui::CollapsingHeader("FOV"))
        {
            float fov = frustum.HorizontalFov();
            ImGui::DragFloat("FOV", &fov, 0.2f, 50.0f, 120.0f);
            frustum.SetHorizontalFovAndAspectRatio(fov, 1.3f);
        }
    }

    void Update() override
    {
        if (transformComponent == nullptr)
        {
            return;
        }

        frustum.SetPos(transformComponent->GetPosition());
        float4x4 view = frustum.ViewMatrix();
        float4x4 proj = frustum.ProjectionMatrix();
        view.Transpose();
        proj.Transpose();
        float4x4 clipMatrix = proj * view * owner->GetModelMatrix();
        clipMatrix.Inverse();
        dd::frustum(clipMatrix, float3(1.0f, 1.0f, 1.0f), 0.5f);
    }

    void OnSave(Json::Value &owner) override
    {
        Json::Value cameraJson;
        cameraJson["type"] = static_cast<int>(ComponentTypes::CAMERA);

        Json::Value frontJson = Json::arrayValue;
        frontJson.append(frustum.Front().x);
        frontJson.append(frustum.Front().y);
        frontJson.append(frustum.Front().z);

        Json::Value upJson = Json::arrayValue;
        upJson.append(frustum.Up().x);
        upJson.append(frustum.Up().y);
        upJson.append(frustum.Up().z);

        cameraJson["front"] = frontJson;
        cameraJson["up"] = upJson;
        cameraJson["nearPlaneDistance"] = frustum.NearPlaneDistance();
        cameraJson["farPlaneDistance"] = frustum.FarPlaneDistance();

        cameraJson["horizontalFov"] = frustum.HorizontalFov();
        cameraJson["verticalFov"] = frustum.VerticalFov();

        owner["components"].append(cameraJson);
    }

    void OnLoad(const Json::Value& componentJson) override
    {
        frustum.SetFront( float3(
            componentJson["front"][0].asFloat(),
            componentJson["front"][1].asFloat(),
            componentJson["front"][2].asFloat()
            ));
        
        frustum.SetUp( float3(
            componentJson["up"][0].asFloat(),
            componentJson["up"][1].asFloat(),
            componentJson["up"][2].asFloat()
            ));

        frustum.SetHorizontalFovAndAspectRatio(componentJson["horizontalFov"].asFloat(),1.3f);
        frustum.SetViewPlaneDistances(componentJson["nearPlaneDistance"].asFloat(),componentJson["farPlaneDistance"].asFloat());
    }

protected:
    Frustum frustum;
    ComponentTransform *transformComponent;
};

#pragma once
#pragma once
