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
    ComponentCamera(GameObject* owner = nullptr, ComponentTypes type = ComponentTypes::CAMERA) : Component(owner, type) {
        transformComponent = owner->GetTransformComponent();
        if (transformComponent == nullptr) {
            return;
        }

      
        
       
        //initialRadiansAngle = radiansAngle = DEGTORAD(0.05);
  

        //Setting frustum
        frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
        frustum.SetViewPlaneDistances(0.1f, 10.0f);
        frustum.SetHorizontalFovAndAspectRatio(50.0f, 1.3f);
        frustum.SetFrame(transformComponent->GetPosition(), -float3::unitZ, float3::unitY);
        
    };


    void OnEditor() override
    {
        float fov = frustum.HorizontalFov();

        if (ImGui::CollapsingHeader("FOV"))
        {
            ImGui::DragFloat("FOV", &fov,0.2f,50.0f,120.0f);
            
        }

        frustum.SetHorizontalFovAndAspectRatio(fov, 1.3f);


    }

    void Update() override {
        if (transformComponent == nullptr) {
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

protected:
    Frustum frustum;
    ComponentTransform* transformComponent;
};

#pragma once
#pragma once
