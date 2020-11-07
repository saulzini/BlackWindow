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
	void RenderVBO(unsigned vbo, unsigned program);
	void RenderTriangle();
	void DestroyVBO(unsigned vbo);

	unsigned int getProgram() const {
		return program;
	};

	unsigned int getVbo() const {
		return vbo;
	};

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	

private:
	unsigned int program;
	unsigned int vbo;
};

