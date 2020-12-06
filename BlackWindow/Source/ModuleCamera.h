#pragma once
#include "Module.h"
#include "Globals.h"
#include "Geometry/Frustum.h"
#include "Math/float3.h"
#include "Core/Point.h"

#define DEGTORAD(angleDegrees) ((angleDegrees) * pi / 180.0)

class ModuleCamera :
	public Module
{


public:
	float3 cameraPosition;
	float movementSpeed;

protected:
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
	iPoint mousePosition;

	float GetMovementSpeedFactor() const;
	float GetZoomSpeedFactor() const;
	float GetTurnSpeedFactor() const;
	float GetRadiansAngleSpeedFactor() const;
	float GetRadiansOrbit() const;


public:
	ModuleCamera();
	~ModuleCamera();

	bool Init() override;
	update_status PreUpdate(float deltaTime) override;
	update_status Update(float deltaTime) override;
	update_status PostUpdate(float deltaTime) override;
	bool CleanUp() override;
	
	float GetMovementSpeed() const  {
		return movementSpeed;
	}

	void SetMovementSpeed(float value) {
		movementSpeed = value;
	}


	void SetTurnSpeed(float value) {
		turnSpeed = value;
	}

	float GetTurnSpeed() const {
		return turnSpeed;
	}

	void SetZoomSpeed(float value) {
		zoomSpeed = value;
	}

	float GetZoomSpeed() const {
		return zoomSpeed;
	}

	void SetFOV(float horizontalFov, float aspectRatio);


	float4x4 GetView() const{
		return frustum.ViewMatrix();
	};

	float4x4 GetViewProjectionM() const {
		return frustum.ViewProjMatrix();
	};

	float4x4 GetProjection() const {
		return frustum.ProjectionMatrix();
	};

	void SetSpeedFactor(float factor) {
		if (factor < 0) {
			return;
		}
		speedFactor = factor;
	}

	void ResetCameraPosition();
	void ResetCamera();
	void ResetToDefaultSpeeds();

	float3 GetCameraPosition() const{
		return cameraPosition;
	}

	void SetCameraPosition(const float3& mCameraPosition);
	
	void RotateAroundPoint(const float3& point,const float3& pivot,const float anglesX, const float anglesY);
	void LookAt(const float3& point);
	
	void WindowResized(int width,int height);
	void SetAspectRatio(float aspectRatio);

	void MoveAccordingNewModelInScene(const float3& dimensions);

	float3 GetFrontVector() const{
		return frustum.Front();
	}
	void SetFrontVector(float3 front){
		frustum.SetFront(front);
	}

	float3 GetUpVector() const{
		return frustum.Up();
	}
	void SetUpVector(float3 Up){
		frustum.SetUp(Up);
	}

	float GetNearPlaneDistance() const{
		return frustum.NearPlaneDistance();
	}
	void SetNearPlaneDistance(float nearPlane);

	float GetFarPlaneDistance() const{
		return frustum.FarPlaneDistance();
	}
	void SetFarPlaneDistance(float farPlane);


	float GetHorizontalFieldOfView() const{
		return frustum.HorizontalFov();
	}
	float GetHorizontalDegreesFieldOfView() const{
		return frustum.HorizontalFov() * 180.f/pi;
	}
	void SetHorizontalDegreesFieldOfView(float degreesHorizontalFOV);

	float GetAspectRatio() const{
		return frustum.AspectRatio();
	}

private:
	void MoveForward(float deltaTime);
	void MoveRight(float deltaTime);
	void MoveUp(float deltaTime);
	void Pitch(float deltaTime);
	void Yaw(float deltaTime);
	void CheckForResetCameraPosition();

	void Rotate(const float3x3& rotationMatrix);
	
	void MousePitch(float deltaTime);
	void MouseZoom(float deltaTime);
	void OrbitCamera(float deltaTime);
	void RotatePitch(float radians,float deltaTime);
	
};

