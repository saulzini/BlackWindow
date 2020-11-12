#include "ModuleRenderExercise.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "Texture2D.h"
#include "IL/il.h"
#include "MathGeoLib-master/src/Math/float4x4.h"
#include "ModuleCamera.h"
ModuleRenderExercise::ModuleRenderExercise()
{
}

// This function must be called one time at creation of vertex buffer
unsigned ModuleRenderExercise::CreateTriangleVBO()
{
	float vtx_data[] = { 
		-1.0f, -1.0f, 0.0f, 
		1.0f, -1.0f, 0.0f, 
		0.0f, 1.0f, 0.0f,

		0.0f,0.0f,
		1.0f,0.0f,
		0.5f,1.0f,
	};
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
	// TODO: retrieve model view and projection
	glUseProgram(program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * 3)); // buffer offset);
	glEnableVertexAttribArray(1);

	float4x4 model, view, projection;

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &projection[0][0]);
	glUniform1i(glGetUniformLocation(program, "mytexture"), 0);

	

	// TODO: bind buffer and vertex attributes
	glDrawArrays(GL_TRIANGLES, 0, 3);



	//glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//glEnableVertexAttribArray(0);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	
	// 1 triangle to draw = 3 vertices

	//glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, (const float*)&model);
	//glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (const float*)&view);
	//glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, (const float*)&proj);

	//glActiveTexture(GL_TEXTURE0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * 3)); // buffer offset

	//glBindTexture(GL_TEXTURE_2D, texture);
	//glUniform1i(glGetUniformLocation(program, "mytexture"), 0);

	//glDrawArrays(GL_TRIANGLES, 0, 3);

}

void ModuleRenderExercise::RenderTriangle()
{
	RenderVBO(vbo, program);
}



bool ModuleRenderExercise::Init()
{
	vbo = CreateTriangleVBO();
	program = App->program->CreateProgramFromSource("HelloWorld.vert", "HelloWorld.frag");
	
	Texture2D texture2D = Texture2D();
	texture = texture2D.loadTexture("Lenna.png");

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
