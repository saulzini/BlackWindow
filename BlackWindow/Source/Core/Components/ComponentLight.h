#pragma once
#include "Core/Components/Component.h"
#include "Core/Components/ComponentTypes.h"
#include <Math/float3.h>
#include "imgui.h"
#include "debugdraw.h"
#include "Application.h"
#include "Math/Quat.h"
//#include "Core/GameObject/GameObject.h"

class ComponentLight : public Component
{
public:
    ComponentLight(GameObject* owner = nullptr, ComponentTypes type = ComponentTypes::LIGHT) : Component(owner, type) {
        componentTransform = owner->GetTransformComponent();
      
    };

   
   

    void Update() override {

        if (componentTransform == nullptr) {
            return;
        }      
        float3 arrowFrom = componentTransform->GetPosition();
        direction = float3(1.0f, 1.0f, 0.0f);
        float3 arrowTo = arrowFrom + direction;
        float radius = 0.5f;
        float3 offset;
        dd::sphere(arrowFrom, dd::colors::Yellow, 1.f, 1.0f);
       
    }

    

protected:

    ComponentTransform* componentTransform;
    float azimuth = 0.0f;
    float polar = 0.0f;
    float3 direction;
    float3 rotation;
    float3 specular;
    float3 diffuse;
    float3 ambient;
};

#pragma once
