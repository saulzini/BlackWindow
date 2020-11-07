#include "ModuleRenderExercise.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleProgram.h"




ModuleRenderExercise::ModuleRenderExercise()
{
}

// This function must be called one time at creation of vertex buffer
unsigned ModuleRenderExercise::CreateTriangleVBO()
{
	float vtx_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
	unsigned int vbo;
	glGenBuffers(1, &vbo); //creates memory in graphic card
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);
	return vbo;
}

// This function must be called one time at destruction of vertex buffer
void ModuleRenderExercise::DestroyVBO(unsigned vbo)
{
	glDeleteBuffers(1, &vbo);
}


void ModuleRenderExercise::RenderVBO(unsigned vbo, unsigned program)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// 1 triangle to draw = 3 vertices
	glUseProgram(program);
	glDrawArrays(GL_TRIANGLES, 0, 3);

}

void ModuleRenderExercise::RenderTriangle()
{
	RenderVBO(vbo, program);
}



bool ModuleRenderExercise::Init()
{
	vbo = CreateTriangleVBO();
	program = App->program->CreateProgramFromSource("HelloWorld.vert", "HelloWorld.frag");
	return true;
}

update_status ModuleRenderExercise::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
	DestroyVBO(vbo);
	return true;
}
