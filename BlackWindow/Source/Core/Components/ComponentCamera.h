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
        frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
        frustum.SetViewPlaneDistances(0.1f, 10.0f);
        frustum.SetHorizontalFovAndAspectRatio(50.0f, 1.3f);
        frustum.SetFrame(transformComponent->GetPosition(), -float3::unitZ, float3::unitY);
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
        frustum.SetFrame(transformComponent->GetPosition(), -float3::unitZ, float3::unitY);

        float4x4 view = frustum.ViewMatrix();
        float4x4 proj = frustum.ProjectionMatrix();
        view = float4x4::identity;
        // view.Transpose();
        float4x4 clipMatrix = proj * -view * owner->GetModelMatrix();
        dd::frustum(clipMatrix.Inverted(), float3(1.0f, 1.0f, 1.0f), 1.0f);
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

protected:
    Frustum frustum;
    ComponentTransform *transformComponent;
};

#pragma once
#pragma once
