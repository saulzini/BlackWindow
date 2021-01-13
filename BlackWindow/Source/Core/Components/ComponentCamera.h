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
        owner->GetTransformComponent();
        if (owner->GetTransformComponent()) {
            owner->GetTransformComponent();
        }
        else {

        }
        position = float3(1, 0, 0);
        orbitPosition = float3(1, 1, 1);
        initialTurnSpeed = turnSpeed = 0.0009f;
        radiansOrbit = initialRadiansOrbit = 0.009f;
        initialMovementSpeed =  zoomSpeed = initialZoomSpeed = 0.005f;
        //initialRadiansAngle = radiansAngle = DEGTORAD(0.05);
        speedFactor = 2.0f;

        //Setting frustum
        frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
        frustum.SetViewPlaneDistances(0.1f, 10.0f);
        frustum.SetHorizontalFovAndAspectRatio(50.0f, 1.3f);
        frustum.SetFrame(position, -float3::unitZ, float3::unitY);
    
    };

    void Update() override {
        frustum.SetFrame(owner->GetTransformComponent()->GetPosition(), -float3::unitZ, float3::unitY);
  
        float4x4 view = frustum.ViewMatrix(); //<-- Important to transpose!
   
        float4x4 proj = frustum.ProjectionMatrix();
        float4x4 clipMatrix = proj * view ;
        dd::frustum(clipMatrix.Inverted(), float3(1.0f, 1.0f, 1.0f), 1.0f);

    }

protected:

    float3 position;
    float3 scale;
    float3 rotation;
    float3 direction;
    Quat rotationQuat;

    float3 initialCameraPosition;
    float3 lastCameraPosition;

    Frustum frustum;
    float turnSpeed;
    float zoomSpeed;
    float radiansAngle;
    float radiansOrbit;

    float initialMovementSpeed;
    float initialTurnSpeed;
    float initialZoomSpeed;
    float initialRadiansAngle;
    float initialRadiansOrbit;
    float3 orbitPosition;

    float speedFactor;
    //mouse
};

#pragma once
#pragma once
