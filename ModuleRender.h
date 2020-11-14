#pragma once
#include "Module.h"
#include "Globals.h"
#include <GL/glew.h>
struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;
//#define DEGTORAD(angleDegrees) ((angleDegrees) * M_PI / 180.0)
class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);

	void* GetContext() {
		return context;
	}

private:
	void* context;
	/*unsigned int program;
	unsigned int vbo;*/
};
