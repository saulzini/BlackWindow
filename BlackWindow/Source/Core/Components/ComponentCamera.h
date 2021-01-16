#pragma once
#include "Geometry/Frustum.h"
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
#include "Core/Components/ComponentTransform.h"
#include <Math/float3.h>
#include "imgui.h"
#include "debugdraw.h"
#include "Application.h"
#include "Math/Quat.h"

#include "Core/GameObject/GameObject.h"

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

    void OnEditor() override;
    void Update() override;

    void OnSave(Json::Value &owner) override;

    void OnLoad(const Json::Value &componentJson) override;

protected:
    Frustum frustum;
    ComponentTransform *transformComponent;
};
