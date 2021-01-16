#include "Core/Components/ComponentCamera.h"

void ComponentCamera::OnEditor()
{
    if (ImGui::CollapsingHeader("FOV"))
    {
        float fov = frustum.HorizontalFov();
        ImGui::DragFloat("FOV", &fov, 0.2f, 50.0f, 120.0f);
        frustum.SetHorizontalFovAndAspectRatio(fov, 1.3f);
    }
}

void ComponentCamera::Update()
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

void ComponentCamera::OnSave(Json::Value &owner)
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

void ComponentCamera::OnLoad(const Json::Value &componentJson)
{
    frustum.SetFront(float3(
        componentJson["front"][0].asFloat(),
        componentJson["front"][1].asFloat(),
        componentJson["front"][2].asFloat()));

    frustum.SetUp(float3(
        componentJson["up"][0].asFloat(),
        componentJson["up"][1].asFloat(),
        componentJson["up"][2].asFloat()));

    frustum.SetHorizontalFovAndAspectRatio(componentJson["horizontalFov"].asFloat(), 1.3f);
    frustum.SetViewPlaneDistances(componentJson["nearPlaneDistance"].asFloat(), componentJson["farPlaneDistance"].asFloat());
}
