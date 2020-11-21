#include "ModuleCamera.h"
#include "MathGeoLib-master/src/Math/float3x3.h"
#include "MathGeoLib-master/src/Math/float3x4.h"
#include "MathGeoLib-master/src/Math/Quat.h"
#include "SDL.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
ModuleCamera::ModuleCamera()
{
	//initializing
	initialCameraPosition = cameraPosition = lastCameraPosition = orbitPosition = float3(1, 1, 3);
	initialTurnSpeed = turnSpeed = 0.0009f;
	initialMovementSpeed = movementSpeed = 0.005f;
	initialRadiansAngle = radiansAngle = DEGTORAD(0.05);
	mousePosition = iPoint(0, 0);
	speedFactor = 2.0f;

	//Setting frustum
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 200.0f);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD(1) * 90.0f, 1.3f);
	ResetCameraPosition();
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	return true;
}

update_status ModuleCamera::PreUpdate(float deltaTime)
{
	return UPDATE_CONTINUE;
}


update_status ModuleCamera::Update(float deltaTime)
{
	
	/*if (
		lastCameraPosition.x != cameraPosition.x || 
		lastCameraPosition.y != cameraPosition.y || 
		lastCameraPosition.z == cameraPosition.z) {
		frustum.SetPos(cameraPosition);
	}*/

	//Setting projection
	float4x4 projectionGL = frustum.ProjectionMatrix().Transposed(); //<-- Important to transpose!
	
	//Send the frustum projection matrix to OpenGL
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*(projectionGL.v));


	// Check for reset camera position
	CheckForResetCameraPosition();

	//Keyboard movements
	MoveForward(deltaTime);
	MoveRight(deltaTime);
	MoveUp(deltaTime);
	Pitch(deltaTime);
	Yaw(deltaTime);
	//Mouse movements
	MousePitch(deltaTime);
	MouseZoom(deltaTime);
	//Orbit 
	OrbitCamera(deltaTime);
	
	
	

	//Passing view matrix to opengl
	float4x4 view = frustum.ViewMatrix(); //<-- Important to transpose!
	view.Transpose();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*(view.v));

	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate(float deltaTime)
{
	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return true;
}

void ModuleCamera::MoveForward(float deltaTime)
{
	if (App->input->GetKey(SDL_SCANCODE_W)) {
		frustum.Translate(frustum.Front() * GetMovementSpeedFactor() * deltaTime);
		cameraPosition = frustum.Pos();
		LOG("W");
	}
	if (App->input->GetKey(SDL_SCANCODE_S)) {
		frustum.Translate(frustum.Front() * -GetMovementSpeedFactor() * deltaTime);
		cameraPosition = frustum.Pos();
		LOG("S");
	}
}

void ModuleCamera::MoveRight(float deltaTime)
{
	if (App->input->GetKey(SDL_SCANCODE_D)) {
		frustum.Translate(frustum.WorldRight() * GetMovementSpeedFactor() * deltaTime);
		cameraPosition = frustum.Pos();
		LOG("D");
	}
	if (App->input->GetKey(SDL_SCANCODE_A)) {
		frustum.Translate(frustum.WorldRight() * -GetMovementSpeedFactor() * deltaTime);
		cameraPosition = frustum.Pos();
		LOG("A");
	}
}

void ModuleCamera::MoveUp(float deltaTime)
{

	if (App->input->GetKey(SDL_SCANCODE_Q)) {
		cameraPosition = float3(cameraPosition.x , cameraPosition.y + GetMovementSpeedFactor() * deltaTime, cameraPosition.z);
		frustum.SetPos(cameraPosition);
		LOG("Q");
	}
	if (App->input->GetKey(SDL_SCANCODE_E)) {
		cameraPosition = float3(cameraPosition.x, cameraPosition.y - GetMovementSpeedFactor() * deltaTime, cameraPosition.z);
		frustum.SetPos(cameraPosition);
		LOG("E");
	}
}

void ModuleCamera::Pitch(float deltaTime)
{
	if (App->input->GetKey(SDL_SCANCODE_UP)) {
		RotatePitch(GetRadiansAngleSpeedFactor(), deltaTime);
		LOG("Up");
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN)) {
		RotatePitch(-GetRadiansAngleSpeedFactor(), deltaTime);
		LOG("Down");
	}
}


void ModuleCamera::RotatePitch(float radians, float deltaTime)
{
	//Reference
	//https://stackoverflow.com/questions/15208104/opengl-camera-pitch-yaw-and-roll-rotation
	float3 lookAtVector = frustum.Front() * cos(radians * deltaTime) + frustum.Up() * sin(radians * deltaTime);
	lookAtVector.Normalize();
	float3 upVector = frustum.WorldRight().Cross(lookAtVector);
	frustum.SetFront(lookAtVector);
	frustum.SetUp(upVector);
}


void ModuleCamera::Yaw(float deltaTime)
{
	if (App->input->GetKey(SDL_SCANCODE_LEFT)) {
		Rotate(frustum.WorldMatrix().RotatePart().RotateY(GetTurnSpeedFactor() * deltaTime));
		LOG("left");
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT)) {
		Rotate(frustum.WorldMatrix().RotatePart().RotateY(-GetTurnSpeedFactor() * deltaTime));
		LOG("right");
	}
	
}

void ModuleCamera::CheckForResetCameraPosition()
{
	if (App->input->GetKey(SDL_SCANCODE_F)) {
		LOG("F");
		ResetCameraPosition();
	}
}

void ModuleCamera::Rotate(const float3x3 rotationMatrix)
{
	vec oldFront = frustum.Front().Normalized();
	frustum.SetFront(rotationMatrix.MulDir(oldFront));
	vec oldUp = frustum.Up().Normalized();
	frustum.SetUp(rotationMatrix.MulDir(oldUp));
}

void ModuleCamera::MousePitch(float deltaTime)
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT)) {
		LOG("Mouse R");

		iPoint newMousePosition = App->input->GetMouseMotion();
		//Checking direction
		//Horizontal
		int result = mousePosition.x - newMousePosition.x;
		// turn right / left direction given by result 
		Rotate(frustum.WorldMatrix().RotatePart().RotateY( result * GetTurnSpeedFactor() * deltaTime));
		
		// Vertical
		result =  mousePosition.y - newMousePosition.y;
		//turn up/down direction given by result
		RotatePitch(result * GetRadiansAngleSpeedFactor(), deltaTime);
	}

}

void ModuleCamera::MouseZoom(float deltaTime)
{
	int scrollAmount =  App->input->GetScrollAmount();
	if (scrollAmount != 0) {
		frustum.Translate(frustum.Front() * (scrollAmount + GetMovementSpeedFactor() ) * deltaTime);
		cameraPosition = frustum.Pos();
	}
}

void ModuleCamera::OrbitCamera(float deltaTime)
{
	//if (App->input->GetKey(SDL_SCANCODE_LALT) && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT)) {

	if (App->input->GetKey(SDL_SCANCODE_LALT)) {
		LOG("Orbit");

		iPoint newMousePosition = App->input->GetMouseMotion();
		float3 oldCameraPosition = float3(cameraPosition);
		int directionX =	(mousePosition.x - newMousePosition.x );
		int directionY =  (mousePosition.y - newMousePosition.y);
		// float rotationAroundYAxis = -directionX * GetTurnSpeedFactor() * deltaTime ; // camera moves horizontally
		// float rotationAroundXAxis = directionY * GetTurnSpeedFactor() * deltaTime; // camera moves vertically

		float rotationAroundYAxis = -1 * GetTurnSpeedFactor() * deltaTime; // testing
		float rotationAroundXAxis = 1 * GetTurnSpeedFactor() * deltaTime; // testing
			
		float3 dir =  cameraPosition - orbitPosition; // get point direction relative to pivot
   		//dir = Quat::RotateX(rotationAroundXAxis) * dir; // yaw
   		dir = Quat::RotateY(rotationAroundYAxis) * dir; // pitch
   		// point = dir + pivot; // calculate rotated point
		cameraPosition = dir + orbitPosition;
		frustum.SetPos(cameraPosition);


		float3 lookAtVector = float3(orbitPosition - cameraPosition);
		lookAtVector.Normalize();
		float3 upVector = frustum.WorldRight().Cross(lookAtVector);
		frustum.SetFront(lookAtVector);
		frustum.SetUp(upVector);

		// float3 vectorA = float3(orbitPosition - cameraPosition);
		// float3 vectorB = float3(orbitPosition - oldCameraPosition);
		// float angleDifference = vectorB.AngleBetweenNorm(vectorA);
		// //float angleDifference  = orbitPosition.AngleBetweenNorm(cameraPosition);

		// Rotate(frustum.WorldMatrix().RotatePart().RotateY( -angleDifference * GetTurnSpeedFactor() * deltaTime));


		// Quat looktAt = Quat::RotateFromTo(frustum.Front(),orbitPosition,frustum.Up(),float3::unitY);
		// float3 vector = looktAt.Transform(dir);
		// vector.Normalize();
		// float3 upVector = frustum.WorldRight().Cross(vector);
		// frustum.SetFront(vector);
		// frustum.SetUp(upVector);

		// Quat looktAt = Quat::RotateFromTo(frustum.Front(),orbitPosition,frustum.Up(),float3::unitY);
		
		// float3 lookAtVector = looktAt *cameraPosition ;
		// lookAtVector.Normalize();
		// frustum.SetFront(-lookAtVector);
		//frustum.SetUp(float3::unitY);

		// setting look at
		// Quat looktAt = Quat::LookAt(frustum.Front(),orbitPosition,frustum.Up(),float3::unitY);
		// float3 lookAtVector = looktAt *cameraPosition ;
		// lookAtVector.Normalize();
		// frustum.SetPos(cameraPosition);
		// frustum.SetFront(lookAtVector);

		


		

	}
}

const float ModuleCamera::GetMovementSpeedFactor()
{
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) || App->input->GetKey(SDL_SCANCODE_RSHIFT) ) {
		return movementSpeed * speedFactor;
	}
	return movementSpeed;
}

const float ModuleCamera::GetTurnSpeedFactor()
{
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) || App->input->GetKey(SDL_SCANCODE_RSHIFT)) {
		return turnSpeed * speedFactor;
	}
	return turnSpeed;
}

const float ModuleCamera::GetRadiansAngleSpeedFactor()
{
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) || App->input->GetKey(SDL_SCANCODE_RSHIFT)) {
		return radiansAngle * speedFactor;
	}
	return radiansAngle;
}

void ModuleCamera::ResetCameraPosition()
{
	frustum.SetPos(initialCameraPosition);
	frustum.SetFront(-float3::unitZ);
	frustum.SetUp(float3::unitY);
}

void ModuleCamera::ResetToDefaultSpeeds()
{
	turnSpeed = initialTurnSpeed;
	movementSpeed = initialMovementSpeed;
	radiansAngle = initialRadiansAngle;
}


