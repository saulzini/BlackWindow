#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib-master/src/Geometry/Frustum.h"
#include "MathGeoLib-master/src/Math/float3.h"
#include "Point.h"
#define DEGTORAD(angleDegrees) ((angleDegrees) * M_PI / 180.0)

class ModuleCamera :
	public Module
{

public:
	float3 cameraPosition;
	float movementSpeed;

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

	const float getMovementSpeed()  {
		return movementSpeed;
	}

	const float getTurnSpeed()  {
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

	void setSpeedFactor(float factor) {
		if (factor < 0) {
			return;
		}
		speedFactor = factor;
	}

	void ResetCameraPosition();
	void ResetToDefaultSpeeds();

	float3 GetCameraPosition() {
		return cameraPosition;
	}

	void SetCameraPosition(float3 mCameraPosition) {
		cameraPosition = mCameraPosition;
	}
	
	void RotateAroundPoint(const float3& point,const float3& pivot,const float anglesX, const float anglesY);
	void LookAt(const float3& point);

	
	void WindowResized(int width,int height);
	void SetAspectRatio(float aspectRatio);

private:
	void MoveForward(float deltaTime);
	void MoveRight(float deltaTime);
	void MoveUp(float deltaTime);
	void Pitch(float deltaTime);
	void Yaw(float deltaTime);
	void CheckForResetCameraPosition();

	void Rotate(const float3x3 rotationMatrix);
	
	void MousePitch(float deltaTime);
	void MouseZoom(float deltaTime);
	void OrbitCamera(float deltaTime);
	void RotatePitch(float radians,float deltaTime);

	const float GetMovementSpeedFactor();
	const float GetTurnSpeedFactor();
	const float GetRadiansAngleSpeedFactor();
	const float GetRadiansOrbit();
	
protected:
	Frustum frustum;
	float turnSpeed;
	float radiansAngle;
	float3 lastCameraPosition;
	float radiansOrbit;

	float initialMovementSpeed;
	float initialTurnSpeed;
	float initialRadiansAngle;
	float initialRadiansOrbit;
	float3 initialCameraPosition;
	float3 orbitPosition;

	float speedFactor;
	//mouse
	iPoint mousePosition;
};

