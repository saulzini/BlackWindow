#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleDebugDraw.h"
#include "SDL.h"
#include "GL/glew.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "ModuleWorld.h"
#include "Leaks.h"
void __stdcall OurOpenGLErrorFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	const char* tmp_source = "", * tmp_type = "", * tmp_severity = "";
	switch (source) {
	case GL_DEBUG_SOURCE_API: tmp_source = "API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: tmp_source = "Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: tmp_source = "Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: tmp_source = "Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION: tmp_source = "Application"; break;
	case GL_DEBUG_SOURCE_OTHER: tmp_source = "Other"; break;
	};
	switch (type) {
	case GL_DEBUG_TYPE_ERROR: tmp_type = "Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: tmp_type = "Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: tmp_type = "Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY: tmp_type = "Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE: tmp_type = "Performance"; break;
	case GL_DEBUG_TYPE_MARKER: tmp_type = "Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP: tmp_type = "Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP: tmp_type = "Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER: tmp_type = "Other"; break;
	};
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: tmp_severity = "high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM: tmp_severity = "medium"; break;
	case GL_DEBUG_SEVERITY_LOW: tmp_severity = "low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: tmp_severity = "notification"; break;
	};
	LOG("<Source:%s> <Type:%s> <Severity:%s> <ID:%d> <Message:%s>\n", tmp_source, tmp_type, tmp_severity, id, message);

}


ModuleRender::ModuleRender()
{	
	glCullFaceCap = true;
	glDepthTestCap = true;
	glAlphaTestCap = false;
	glBlendCap = false;
}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{


	context = SDL_GL_CreateContext(App->window->window); //init context with window
	//SDL_WINDOW_OPENGL(SDL_WINDOW_SHOWN);

	LOG("Creating Renderer context");
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return false;
	}

	// LOG("Vendor: %s", glGetString(GL_VENDOR));
	// LOG("Renderer: %s", glGetString(GL_RENDERER));
	// LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	// LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	glDebugMessageCallback(&OurOpenGLErrorFunction, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);

	if (glDepthTestCap){
		glEnable(GL_DEPTH_TEST); // Enable depth test
	}
	if (glCullFaceCap){
		glEnable(GL_CULL_FACE); // Enable cull backward faces
	}
	glFrontFace(GL_CCW); // Front faces will be counter clockwise

	return true;
}

update_status ModuleRender::PreUpdate(float deltaTime)
{
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update(float deltaTime)
{

	//glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	int w = 0;
	int h = 0;
	float4x4 proj = App->camera->GetProjection();
	float4x4 view = App->camera->GetView();
	SDL_GetWindowSize(App->window->window, &w, &h);
	glClearColor(0.439f, 0.439f, 0.439f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	App->draw->Update();
	App->draw->Draw(view, proj, w, h);

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate(float deltaTime)
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");
	//Destroy window
	SDL_GL_DeleteContext(context);
	context =nullptr;
	return true;
}

void ModuleRender::SetGlCullFaceCap(bool value) 
{
	if (glCullFaceCap == value){
		return;
	}
	glCullFaceCap = value;
	if (glCullFaceCap){
		glEnable(GL_CULL_FACE);
	}
	else {
		glDisable(GL_CULL_FACE);
	}
}

void ModuleRender::SetGlAlphaTestCap(bool value) 
{
	if (glAlphaTestCap == value){
		return;
	}
	glAlphaTestCap = value;
	if (glAlphaTestCap){
		glEnable(GL_ALPHA_TEST);
	}
	else {
		glDisable(GL_ALPHA_TEST);
	}
}

void ModuleRender::SetGlBlendCap(bool value) 
{
	if (glBlendCap == value){
		return;
	}
	glBlendCap = value;
	if (glBlendCap){
		glEnable(GL_BLEND);
	}
	else {
		glDisable(GL_BLEND);
	}
}

void ModuleRender::SetGlDepthTestCap(bool value) 
{
	if (glDepthTestCap == value){
		return;
	}
	glDepthTestCap = value;
	if (glDepthTestCap){
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}
}