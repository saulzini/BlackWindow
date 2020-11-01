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

	//Setting View 
	MoveForward();
	MoveRight();
	MoveUp();
	Roll();
	Pitch();
	Yaw();
	

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
		vec front = frustum.Front();
		frustum.Translate(front * movement_speed);
		camera_position = frustum.Pos();
		LOG("W");
	}
	if (App->input->GetKey(SDL_SCANCODE_S)) {
		vec front = frustum.Front();
		frustum.Translate(front * -movement_speed);
		camera_position = frustum.Pos();
		LOG("S");
	}
}

void ModuleCamera::MoveRight()
{
	if (App->input->GetKey(SDL_SCANCODE_D)) {
		camera_position = float3(camera_position.x - movement_speed, camera_position.y, camera_position.z );
		frustum.SetPos(camera_position);
		LOG("D");
	}
	if (App->input->GetKey(SDL_SCANCODE_A)) {
		camera_position = float3(camera_position.x + movement_speed, camera_position.y, camera_position.z );
		frustum.SetPos(camera_position);
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

void ModuleCamera::Roll()
{
	/*if (App->input->GetKey(SDL_SCANCODE_UP)) {
		float3x3 rotationMatrix = float3x3::RotateY(turn_speed);
		vec oldFront = frustum.Front().Normalized();
		frustum.SetFront(rotationMatrix.MulDir(oldFront));
		vec oldUp = frustum.Up().Normalized();
		frustum.SetUp(rotationMatrix.MulDir(oldUp));
		LOG("Up");
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN)) {
		float3x3 rotationMatrix = float3x3::RotateY(-turn_speed);
		vec oldFront = frustum.Front().Normalized();
		frustum.SetFront(rotationMatrix.MulDir(oldFront));
		vec oldUp = frustum.Up().Normalized();
		frustum.SetUp(rotationMatrix.MulDir(oldUp));
		LOG("Down");
	}*/
}

void ModuleCamera::Pitch()
{
	if (App->input->GetKey(SDL_SCANCODE_UP)) {
		float3x3 rotationMatrix = float3x3::RotateY(turn_speed);
		vec oldFront = frustum.Front().Normalized();
		frustum.SetFront(rotationMatrix.MulDir(oldFront));
		vec oldUp = frustum.Up().Normalized();
		frustum.SetUp(rotationMatrix.MulDir(oldUp));
		LOG("Up");
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN)) {
		float3x3 rotationMatrix = float3x3::RotateY(-turn_speed);
		vec oldFront = frustum.Front().Normalized();
		frustum.SetFront(rotationMatrix.MulDir(oldFront));
		vec oldUp = frustum.Up().Normalized();
		frustum.SetUp(rotationMatrix.MulDir(oldUp));
		LOG("Down");
	}
}

void ModuleCamera::Yaw()
{
	if (App->input->GetKey(SDL_SCANCODE_LEFT)) {
		float3x3 rotationMatrix = float3x3::RotateZ(-turn_speed);
		vec oldFront = frustum.Front().Normalized();
		frustum.SetFront(rotationMatrix.MulDir(oldFront));
		vec oldUp = frustum.Up().Normalized();
		frustum.SetUp(rotationMatrix.MulDir(oldUp));
		LOG("Left");
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT)) {
		float3x3 rotationMatrix = float3x3::RotateZ(turn_speed);
		vec oldFront = frustum.Front().Normalized();
		frustum.SetFront(rotationMatrix.MulDir(oldFront));
		vec oldUp = frustum.Up().Normalized();
		frustum.SetUp(rotationMatrix.MulDir(oldUp));
		LOG("Right");
	}
}
