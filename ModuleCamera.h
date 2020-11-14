#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib-master/src/Geometry/Frustum.h"
#include "MathGeoLib-master/src/Math/float3.h"
#include "Point.h"
#define DEGTORAD(angleDegrees) ((angleDegrees) * M_PI / 180.0)

//class Frustum;
//class float3;
class ModuleCamera :
	public Module
{

public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status PreUpdate(float deltaTime);
	update_status Update(float deltaTime);
	update_status PostUpdate(float deltaTime);
	bool CleanUp();
	
	void setMovementSpeed(float value) {
		if (value <= 0.0f) {
			return;
		}
		movementSpeed = value;
	}

	void setTurnSpeed(float value) {
		if (value <= 0.0f) {
			return;
		}
		turnSpeed = value;
	}

	float getMovementSpeed() const {
		return movementSpeed;
	}

	float getTurnSpeed() const {
		return turnSpeed;
	}

	void SetFOV(float horizontalFov, float aspectRatio) {
		frustum.SetHorizontalFovAndAspectRatio(horizontalFov, aspectRatio);
	}


	const float4x4 GetView() {
		return frustum.ViewMatrix();
	};


	const float4x4 GetProjection() {
		return frustum.ProjectionMatrix();
	};
private:
	void MoveForward(float deltaTime);
	void MoveRight(float deltaTime);
	void MoveUp(float deltaTime);
	void Pitch(float deltaTime);
	void Yaw(float deltaTime);

	void Rotate(const float3x3 rotation_matrix);
	void RotatePitch(float radians,float deltaTime);
	void MousePitch(float deltaTime);

protected:
	Frustum frustum;
	float movementSpeed;
	float turnSpeed;
	float radiansAngle;
	float3 cameraPosition;
	//mouse
	iPoint mousePosition;
};

