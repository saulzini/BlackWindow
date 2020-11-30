#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "Leaks.h"
ModuleWindow::ModuleWindow()
{
	maxWidth = width = SCREEN_WIDTH;
	maxHeight = height = SCREEN_HEIGHT;
	brightness = 1;
	refreshRate = 0;

	fullscreen = false;
	resizable = true;
	border = true;
	fullDesktop = false;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		Uint32 flags =  SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL;

		if (border){
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // desired version
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // we want a double buffer
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // we want to have a depth buffer with 24 bits
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); // we want to have a stencil buffer with 8 bits

		// Checking for screen size
		SDL_DisplayMode displayMode;

		if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0)
		{
			SDL_Log("SDL_GetDesktop DisplayMode failed: %s", SDL_GetError());
			return false;
		}

		maxWidth = displayMode.w;
		maxHeight = displayMode.h;
		refreshRate = displayMode.refresh_rate;
		// Getting size according to display
		width = (int) ((float)displayMode.w*0.8f);
		height = (int)  ((float)displayMode.h*0.8f);

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
			SetBrightness(brightness);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetBrightness(float value) 
{
	if (brightness == value){
		return;
	}
	brightness = value;
	SDL_SetWindowBrightness(window,brightness);
}

void ModuleWindow::SetWidth(int value) 
{
	if (value == width){
		return;
	}
	width = value;
	SDL_SetWindowSize(window,width,height);
}

void ModuleWindow::SetHeight(int value) 
{
	if (value == height){
		return;
	}
	height = value;
	SDL_SetWindowSize(window,width,height);
}


void ModuleWindow::SetWindowDimensions(int valWidth , int valHeight) 
{
	if (width ==valWidth &&  height == valHeight){
		return;
	}
	width = valWidth;
	height = valHeight;
	SDL_SetWindowSize(window,width,height);
}

void ModuleWindow::SetFullscreen(bool value) 
{
	if (fullscreen == value){
		return;
	}
	fullscreen = value;
	SDL_SetWindowFullscreen(window,fullscreen);
}

void ModuleWindow::SetResizable(bool value) 
{
	if (resizable == value){
		return;
	}
	resizable = value;
	SDL_SetWindowResizable(window,(SDL_bool)resizable);
}

void ModuleWindow::SetBorder(bool value) 
{
	if (border == value){
		return;
	}
	border = value;
	SDL_SetWindowBordered(window,(SDL_bool)border);
}

void ModuleWindow::SetFullDesktop(bool value) 
{
	if (fullDesktop ==value){
		return;
	}
	fullDesktop = value;
	SDL_SetWindowSize(window,maxWidth,maxHeight);
}

