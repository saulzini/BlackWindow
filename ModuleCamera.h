#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib-master/src/Geometry/Frustum.h"
#include "MathGeoLib-master/src/Math/float3.h"

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
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void MoveForward();
	void MoveRight();
	void MoveUp();
	void Roll();
	void Pitch();
	void Yaw();

	void setMovementSpeed(float value) {
		if (value <= 0.0f) {
			return;
		}
		movement_speed = value;
	}

	void setTurnSpeed(float value) {
		if (value <= 0.0f) {
			return;
		}
		turn_speed = value;
	}

protected:
	Frustum frustum;
	float movement_speed;
	float turn_speed;
	float3 camera_position;
};

