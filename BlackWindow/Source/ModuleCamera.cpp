#include "ModuleCamera.h"
#include "Math/float3x3.h"
#include "Math/float3x4.h"
#include "Math/Quat.h"
#include "SDL.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleEditor.h"
#include "UIWindow/ConsoleWindow.h"

ModuleCamera::ModuleCamera()
{
	//initializing
	initialCameraPosition = cameraPosition = lastCameraPosition = float3(1, 3, 10);
	orbitPosition = float3(1,1,1);
	initialTurnSpeed = turnSpeed = 0.0009f;
	radiansOrbit = initialRadiansOrbit = 0.009f;
	initialMovementSpeed = movementSpeed = zoomSpeed = initialZoomSpeed = 0.005f;
	initialRadiansAngle = radiansAngle = DEGTORAD(0.05);
	mousePosition = iPoint(0, 0);
	speedFactor = 2.0f;

	//Setting frustum
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 200.0f);
	frustum.SetHorizontalFovAndAspectRatio( 90.0f,1.3f);
	ResetCameraPosition();
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	WindowResized(App->window->getWidth(),App->window->getHeight());
	return true;
}

update_status ModuleCamera::PreUpdate(float deltaTime)
{
	return UPDATE_CONTINUE;
}


update_status ModuleCamera::Update(float deltaTime)
{
	
	if ( //Updating given ui
		lastCameraPosition.x != cameraPosition.x || 
		lastCameraPosition.y != cameraPosition.y || 
		lastCameraPosition.z == cameraPosition.z) {
		frustum.SetPos(cameraPosition);
	}

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

void ModuleCamera::SetFOV(float horizontalFov, float aspectRatio) 
{
	frustum.SetHorizontalFovAndAspectRatio(horizontalFov, aspectRatio);
}


void ModuleCamera::MoveForward(float deltaTime)
{
	if (App->input->GetKey(SDL_SCANCODE_W)) {
		frustum.Translate(frustum.Front() * GetMovementSpeedFactor() * deltaTime);
		cameraPosition = frustum.Pos();
		// LOG("W");
	}
	if (App->input->GetKey(SDL_SCANCODE_S)) {
		frustum.Translate(frustum.Front() * -GetMovementSpeedFactor() * deltaTime);
		cameraPosition = frustum.Pos();
		// LOG("S");
	}
}

void ModuleCamera::MoveRight(float deltaTime)
{
	if (App->input->GetKey(SDL_SCANCODE_D)) {
		frustum.Translate(frustum.WorldRight() * GetMovementSpeedFactor() * deltaTime);
		cameraPosition = frustum.Pos();
		// LOG("D");
	}
	if (App->input->GetKey(SDL_SCANCODE_A)) {
		frustum.Translate(frustum.WorldRight() * -GetMovementSpeedFactor() * deltaTime);
		cameraPosition = frustum.Pos();
		// LOG("A");
	}
}

void ModuleCamera::MoveUp(float deltaTime)
{

	if (App->input->GetKey(SDL_SCANCODE_Q)) {
		cameraPosition = float3(cameraPosition.x , cameraPosition.y + GetMovementSpeedFactor() * deltaTime, cameraPosition.z);
		frustum.SetPos(cameraPosition);
		// LOG("Q");
	}
	if (App->input->GetKey(SDL_SCANCODE_E)) {
		cameraPosition = float3(cameraPosition.x, cameraPosition.y - GetMovementSpeedFactor() * deltaTime, cameraPosition.z);
		frustum.SetPos(cameraPosition);
		// LOG("E");
	}
}

void ModuleCamera::Pitch(float deltaTime)
{
	if (App->input->GetKey(SDL_SCANCODE_UP)) {
		RotatePitch(GetRadiansAngleSpeedFactor(), deltaTime);
		// LOG("Up");
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN)) {
		RotatePitch(-GetRadiansAngleSpeedFactor(), deltaTime);
		// LOG("Down");
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
		// LOG("left");
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT)) {
		Rotate(frustum.WorldMatrix().RotatePart().RotateY(-GetTurnSpeedFactor() * deltaTime));
		// LOG("right");
	}
	
}

void ModuleCamera::CheckForResetCameraPosition()
{
	if (App->input->GetKey(SDL_SCANCODE_F)) {
		// LOG("F");
		ResetCameraPosition();
	}
}

void ModuleCamera::Rotate(const float3x3 rotationMatrix)
{
	float3 oldFront = frustum.Front().Normalized();
	frustum.SetFront(rotationMatrix.MulDir(oldFront));
	float3 oldUp = frustum.Up().Normalized();
	frustum.SetUp(rotationMatrix.MulDir(oldUp));
}

void ModuleCamera::MousePitch(float deltaTime)
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT)) {
		// LOG("Mouse R");

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
		frustum.Translate(frustum.Front() * (scrollAmount + GetZoomSpeedFactor() ) * deltaTime);
		cameraPosition = frustum.Pos();
	}
}


void ModuleCamera::RotateAroundPoint(const float3& point, const float3& pivot, const float angleX, const float angleY)
{
	// Calculating point around pivot
	float3 dir = cameraPosition - orbitPosition; // get point direction relative to pivot
	dir = Quat::RotateX(angleX) * dir; // yaw
	dir = Quat::RotateY(angleY) * dir; // pitch
	cameraPosition = dir + orbitPosition; //calculating the new point
	frustum.SetPos(cameraPosition);
}

void ModuleCamera::LookAt(const float3& point)
{
	float3 dir = point - cameraPosition;
	float3x3 m = float3x3::LookAt(frustum.Front(), dir.Normalized(), frustum.Up(), float3::unitY);
	float3 lookAtVector = m.MulDir(frustum.Front()).Normalized();
	float3 upVector = m.MulDir(frustum.Up()).Normalized();
	frustum.SetFront(lookAtVector);
	frustum.SetUp(upVector);
}

/// <summary>
/// The window will mantain the same size when the window is resized
/// </summary>
/// <param name="width"></param>
/// <param name="height"></param>
void ModuleCamera::WindowResized(int width,int height) 
{
	LOG("Window resized %d,%d",width,height);
	
	float newHeight = (width / frustum.AspectRatio()); 

	if (newHeight == 0.0f) {
		App->editor->consoleWindow->AddLog("Error cant set aspect ratio");
		return;
	}
	SetAspectRatio((float)width / (float)newHeight);
}

void ModuleCamera::SetAspectRatio(float aspectRatio) 
{
	frustum.SetVerticalFovAndAspectRatio(frustum.VerticalFov() ,aspectRatio);
}

void ModuleCamera::MoveAccordingNewModelInScene(float3 dimensions) 
{
	// calculate moving closer/near
	float height = dimensions.y;
	float offset = 0.7f;

	frustum.Translate(frustum.Front() * -(height*offset) );
	cameraPosition = frustum.Pos();
	
	LookAt(float3::zero +dimensions); //in this case the object is in the origin so it could be just dimensions
}

void ModuleCamera::OrbitCamera(float deltaTime)
{
	if (App->input->GetKey(SDL_SCANCODE_LALT) && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT)) {
		// LOG("Orbit");
		iPoint newMousePosition = App->input->GetMouseMotion();
		float3 oldCameraPosition = float3(cameraPosition);
		int directionX =	newMousePosition.x;
		int directionY =  newMousePosition.y;
	
		float rotationAroundYAxis = -directionX * GetRadiansOrbit() * deltaTime ; // camera moves horizontally
		float rotationAroundXAxis = directionY * GetRadiansOrbit() * deltaTime; // camera moves vertically
		
		RotateAroundPoint(cameraPosition,orbitPosition,rotationAroundXAxis,rotationAroundYAxis);
		LookAt(orbitPosition);
	}
}

float ModuleCamera::GetMovementSpeedFactor() const
{
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) || App->input->GetKey(SDL_SCANCODE_RSHIFT) ) {
		return movementSpeed * speedFactor;
	}
	return movementSpeed;
}

float ModuleCamera::GetZoomSpeedFactor() const
{
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) || App->input->GetKey(SDL_SCANCODE_RSHIFT) ) {
		return zoomSpeed * speedFactor;
	}
	return zoomSpeed;
}

float ModuleCamera::GetTurnSpeedFactor() const
{
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) || App->input->GetKey(SDL_SCANCODE_RSHIFT)) {
		return turnSpeed * speedFactor;
	}
	return turnSpeed;
}

float ModuleCamera::GetRadiansAngleSpeedFactor() const
{
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) || App->input->GetKey(SDL_SCANCODE_RSHIFT)) {
		return radiansAngle * speedFactor;
	}
	return radiansAngle;
}

float ModuleCamera::GetRadiansOrbit() const
{
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) || App->input->GetKey(SDL_SCANCODE_RSHIFT)) {
		return radiansOrbit * speedFactor;
	}
	return radiansOrbit;
}

void ModuleCamera::ResetCameraPosition()
{
	cameraPosition = initialCameraPosition;
	frustum.SetFrame(cameraPosition,-float3::unitZ,float3::unitY);
	LookAt(float3::zero);

}

void ModuleCamera::ResetCamera() 
{
	frustum.SetViewPlaneDistances(0.1f, 200.0f);
	frustum.SetHorizontalFovAndAspectRatio( 90.0f,1.3f);
	WindowResized(App->window->getWidth(),App->window->getHeight());
	ResetCameraPosition();
	ResetToDefaultSpeeds();
}

void ModuleCamera::ResetToDefaultSpeeds()
{
	turnSpeed = initialTurnSpeed;
	movementSpeed = initialMovementSpeed;
	radiansAngle = initialRadiansAngle;
	zoomSpeed = initialZoomSpeed;
}

void ModuleCamera::SetCameraPosition(float3 mCameraPosition) 
{
	cameraPosition = mCameraPosition;
}


