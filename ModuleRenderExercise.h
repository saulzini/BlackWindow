#pragma once
#include "Module.h"
class ModuleRenderExercise :
	public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();

	//Rendering triangle
	unsigned CreateTriangleVBO();
	void RenderTriangle();
	void DestroyVBO(unsigned vbo);

	unsigned int getProgram() const {
		return program;
	};

	unsigned int getVbo() const {
		return vbo;
	};

	bool Init();
	update_status PreUpdate(float deltaTime);
	update_status Update(float deltaTime);
	update_status PostUpdate(float deltaTime);
	bool CleanUp();

	

private:
	unsigned int program;
	unsigned int vbo;
	unsigned int texture;
};
