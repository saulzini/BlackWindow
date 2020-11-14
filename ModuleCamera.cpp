#include "ModuleCamera.h"
#include "MathGeoLib-master/src/Math/float3x3.h"
#include "SDL.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
ModuleCamera::ModuleCamera()
{
	//initializing
	cameraPosition = float3(0, 1, -2);
	turnSpeed = 0.0005f;
	movementSpeed = 0.005f;
	radiansAngle = DEGTORAD(0.05);
	mousePosition = iPoint(0, 0);

	//Setting frustum
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 200.0f);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD(1) * 90.0f, 1.3f);
	frustum.SetPos(cameraPosition);
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);
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
	//Setting projection
	float4x4 projectionGL = frustum.ProjectionMatrix().Transposed(); //<-- Important to transpose!
	
	//Send the frustum projection matrix to OpenGL
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*(projectionGL.v));

	//Keyboard movements
	MoveForward(deltaTime);
	MoveRight(deltaTime);
	MoveUp(deltaTime);
	Pitch(deltaTime);
	Yaw(deltaTime);
	//Mouse movements
	MousePitch(deltaTime);
	

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
		frustum.Translate(frustum.Front() * movementSpeed * deltaTime);
		cameraPosition = frustum.Pos();
		LOG("W");
	}
	if (App->input->GetKey(SDL_SCANCODE_S)) {
		frustum.Translate(frustum.Front() * -movementSpeed * deltaTime);
		cameraPosition = frustum.Pos();
		LOG("S");
	}
}

void ModuleCamera::MoveRight(float deltaTime)
{
	if (App->input->GetKey(SDL_SCANCODE_D)) {
		frustum.Translate(frustum.WorldRight() * movementSpeed * deltaTime);
		cameraPosition = frustum.Pos();
		LOG("D");
	}
	if (App->input->GetKey(SDL_SCANCODE_A)) {
		frustum.Translate(frustum.WorldRight() * -movementSpeed * deltaTime);
		cameraPosition = frustum.Pos();
		LOG("A");
	}
}

void ModuleCamera::MoveUp(float deltaTime)
{

	if (App->input->GetKey(SDL_SCANCODE_Q)) {
		cameraPosition = float3(cameraPosition.x , cameraPosition.y + movementSpeed * deltaTime, cameraPosition.z);
		frustum.SetPos(cameraPosition);
		LOG("Q");
	}
	if (App->input->GetKey(SDL_SCANCODE_E)) {
		cameraPosition = float3(cameraPosition.x, cameraPosition.y - movementSpeed * deltaTime, cameraPosition.z);
		frustum.SetPos(cameraPosition);
		LOG("E");
	}
}

void ModuleCamera::Pitch(float deltaTime)
{
	if (App->input->GetKey(SDL_SCANCODE_UP)) {
		RotatePitch(radiansAngle, deltaTime);
		LOG("Up");
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN)) {
		RotatePitch(-radiansAngle, deltaTime);
		LOG("Down");
	}
}

void ModuleCamera::Yaw(float deltaTime)
{
	if (App->input->GetKey(SDL_SCANCODE_LEFT)) {
		Rotate(frustum.WorldMatrix().RotatePart().RotateY(turnSpeed));
		LOG("left");
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT)) {
		Rotate(frustum.WorldMatrix().RotatePart().RotateY(-turnSpeed));
		LOG("right");
	}
	
}

void ModuleCamera::Rotate(const float3x3 rotation_matrix)
{
	vec oldFront = frustum.Front().Normalized();
	frustum.SetFront(rotation_matrix.MulDir(oldFront));
	vec oldUp = frustum.Up().Normalized();
	frustum.SetUp(rotation_matrix.MulDir(oldUp));
}

void ModuleCamera::RotatePitch(float radians,float deltaTime)
{
	//Reference
	//https://stackoverflow.com/questions/15208104/opengl-camera-pitch-yaw-and-roll-rotation
	float3 lookAtVector = frustum.Front() * cos(radians*deltaTime) + frustum.Up() * sin(radians * deltaTime) ;
	lookAtVector.Normalize();
	float3 upVector = frustum.WorldRight().Cross(lookAtVector);
	frustum.SetFront(lookAtVector);
	frustum.SetUp(upVector);
}

void ModuleCamera::MousePitch(float deltaTime)
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT)) {
		LOG("Mouse R");

		iPoint new_mousePosition = App->input->GetMouseMotion();
		//Checking direction
		//Horizontal
		int result = mousePosition.x - new_mousePosition.x;
		// turn right / left direction given by result 
		Rotate(frustum.WorldMatrix().RotatePart().RotateY( result *turnSpeed * deltaTime));
		
		// Vertical
		result =  mousePosition.y - new_mousePosition.y;
		//turn up/down direction given by result
		RotatePitch(result * radiansAngle, deltaTime);
	}

}
