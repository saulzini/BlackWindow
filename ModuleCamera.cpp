#include "ModuleCamera.h"
//#include "MathGeoLib-master/src/Geometry/Frustum.h"
#include "MathGeoLib-master/src/Math/float3x3.h"
#include "SDL.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleInput.h"
ModuleCamera::ModuleCamera()
{
	//initializing
	camera_position = float3(0, 1, -2);
	turn_speed = 0.0005f;
	movement_speed = 0.005f;
	radians_angle = DEGTORAD(0.05);
	mouse_position = iPoint(0, 0);

	//Setting frustum
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 200.0f);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD(1) * 90.0f, 1.3f);
	frustum.SetPos(camera_position);
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

update_status ModuleCamera::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
	//Setting projection
	float4x4 projectionGL = frustum.ProjectionMatrix().Transposed(); //<-- Important to transpose!
	
	//Send the frustum projection matrix to OpenGL
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*(projectionGL.v));

	//Keyboard movements
	MoveForward();
	MoveRight();
	MoveUp();
	Pitch();
	Yaw();
	//Mouse movements
	MousePitch();
	

	//Passing view matrix to opengl
	float4x4 view = frustum.ViewMatrix(); //<-- Important to transpose!
	view.Transpose();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*(view.v));
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return true;
}

void ModuleCamera::MoveForward()
{
	if (App->input->GetKey(SDL_SCANCODE_W)) {
		frustum.Translate(frustum.Front() * movement_speed);
		camera_position = frustum.Pos();
		LOG("W");
	}
	if (App->input->GetKey(SDL_SCANCODE_S)) {
		frustum.Translate(frustum.Front() * -movement_speed);
		camera_position = frustum.Pos();
		LOG("S");
	}
}

void ModuleCamera::MoveRight()
{
	if (App->input->GetKey(SDL_SCANCODE_D)) {
		frustum.Translate(frustum.WorldRight() * movement_speed);
		camera_position = frustum.Pos();
		LOG("D");
	}
	if (App->input->GetKey(SDL_SCANCODE_A)) {
		frustum.Translate(frustum.WorldRight() * -movement_speed);
		camera_position = frustum.Pos();
		LOG("A");
	}
}

void ModuleCamera::MoveUp()
{

	if (App->input->GetKey(SDL_SCANCODE_Q)) {
		camera_position = float3(camera_position.x , camera_position.y + movement_speed, camera_position.z);
		frustum.SetPos(camera_position);
		LOG("Q");
	}
	if (App->input->GetKey(SDL_SCANCODE_E)) {
		camera_position = float3(camera_position.x, camera_position.y - movement_speed, camera_position.z);
		frustum.SetPos(camera_position);
		LOG("E");
	}
}

void ModuleCamera::Pitch()
{
	if (App->input->GetKey(SDL_SCANCODE_UP)) {
		//float radians_angle = DEGTORAD(pitch_angle);
		/*float3 lookAtVector = frustum.Front() * cos(radians_angle) + frustum.Up()* sin(radians_angle);
		lookAtVector.Normalize();

		float3 upVector = frustum.WorldRight().Cross(lookAtVector);
		frustum.SetFront(lookAtVector);
		frustum.SetUp(upVector);*/
		RotatePitch(radians_angle);
		LOG("Up");
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN)) {
		//float radians_angle = DEGTORAD(pitch_angle);
		/*float3 lookAtVector = frustum.Front() * cos(-radians_angle) + frustum.Up() * sin(-radians_angle);
		lookAtVector.Normalize();

		float3 upVector = frustum.WorldRight().Cross(lookAtVector);
		frustum.SetFront(lookAtVector);
		frustum.SetUp(upVector);*/
		RotatePitch(-radians_angle);

		LOG("Down");
	}
}

void ModuleCamera::Yaw()
{
	if (App->input->GetKey(SDL_SCANCODE_LEFT)) {
		Rotate(frustum.WorldMatrix().RotatePart().RotateY(turn_speed));
		//Rotate(float3x3::RotateY(turn_speed));
		LOG("left");
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT)) {
		/*float3x3 rotationMatrix = float3x3::RotateY(-turn_speed);
		vec oldFront = frustum.Front().Normalized();
		frustum.SetFront(rotationMatrix.MulDir(oldFront));
		vec oldUp = frustum.Up().Normalized();
		frustum.SetUp(rotationMatrix.MulDir(oldUp));*/
		Rotate(frustum.WorldMatrix().RotatePart().RotateY(-turn_speed));
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

void ModuleCamera::RotatePitch(const float radians)
{
	//Reference
	//https://stackoverflow.com/questions/15208104/opengl-camera-pitch-yaw-and-roll-rotation
	float3 lookAtVector = frustum.Front() * cos(radians) + frustum.Up() * sin(radians);
	lookAtVector.Normalize();
	float3 upVector = frustum.WorldRight().Cross(lookAtVector);
	frustum.SetFront(lookAtVector);
	frustum.SetUp(upVector);
}

void ModuleCamera::MousePitch()
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT)) {
		LOG("Mouse R");

		iPoint new_mouse_position = App->input->GetMouseMotion();
		//Checking direction
		//Horizontal
		int result = mouse_position.x - new_mouse_position.x;
		// turn right / left direction given by result 
		Rotate(frustum.WorldMatrix().RotatePart().RotateY( result *turn_speed));
		
		// Vertical
		result =  mouse_position.y - new_mouse_position.y;
		//turn up/down direction given by result
		RotatePitch(result * radians_angle);
	}

}
