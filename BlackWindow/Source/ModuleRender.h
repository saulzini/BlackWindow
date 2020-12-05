#pragma once
#include "Module.h"
#include "Globals.h"
#include <GL/glew.h>
struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init() override;
	update_status PreUpdate(float deltaTime) override;
	update_status Update(float deltaTime) override;
	update_status PostUpdate(float deltaTime) override;
	bool CleanUp() override;
	void WindowResized(unsigned width, unsigned height);

	void* GetContext() {
		return context;
	}

	bool GetGlCullFaceCap() const{
		return glCullFaceCap;
	}

	void SetGlCullFaceCap(bool value);

	bool GetGlAlphaTestCap() const{
		return glAlphaTestCap;
	}

	void SetGlAlphaTestCap(bool value);

	bool GetGlBlendCap() const{
		return glBlendCap;
	}

	void SetGlBlendCap(bool value);

	bool GetGlDepthTestCap() const{
		return glDepthTestCap;
	}

	void SetGlDepthTestCap(bool value);

private:
	void* context;

	bool glCullFaceCap;
	bool glAlphaTestCap;
	bool glBlendCap;
	bool glDepthTestCap;


};
