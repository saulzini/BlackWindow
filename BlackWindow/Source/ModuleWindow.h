#pragma once

#include "Module.h"
#include "SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	// Called before quitting
	bool CleanUp();



	int GetRefreshRate() const{
		return refreshRate;
	}


	float GetBrightnesss() const{
		return brightness;
	}

	void SetBrightness(float value);

	int GetWidth() const{
		return width;
	}

	void SetWidth(int value);

	int GetHeight() const{
		return height;
	}

	void SetHeight(int value);

	int GetMaxHeight() const{
		return height;
	}

	int GetMaxWidth() const{
		return width;
	}

	void SetWindowDimensions(int width , int height);

	bool GetFullscreen() const{
		return fullscreen;
	}

	void SetFullscreen(bool value);

	bool GetResizable() const{
		return resizable;
	}

	void SetResizable(bool value);

	bool GetBorder() const{
		return border;
	}

	void SetBorder(bool value);

	bool GetFullDesktop() const{
		return fullDesktop;
	}

	void SetFullDesktop(bool value);

public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

private:
	int width;
	int height;

	int maxWidth;
	int maxHeight;
	int refreshRate;
	float brightness;

	bool fullscreen;
	bool resizable;
	bool border;
	bool fullDesktop;
};
