#include "ModuleRenderExercise.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "Texture2D.h"
#include "MathGeoLib-master/src/Math/float4x4.h"
#include "ModuleCamera.h"
ModuleRenderExercise::ModuleRenderExercise()
{
}

// This function must be called one time at creation of vertex buffer
unsigned ModuleRenderExercise::CreateTriangleVBO()
{
	float vtx_data[] = { 
		-1.0f, -1.0f, 0.0f, // ← v0 pos
		 1.0f, -1.0f, 0.0f, // ← v1 pos
		 0.0f, 1.0f, 0.0f, // ← v2 pos
		
		 0.0f, 0.0f, // ← v0 texcoord
		 1.0f, 0.0f, // ← v1 texcoord
		 0.5f, 1.0f // ← v2 texcoord
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


void ModuleRenderExercise::RenderTriangle()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// 1 triangle to draw = 3 vertices
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
		(void*) (sizeof(float) * 3 * 3) // buffer offset
	);

	glUseProgram(program);
	float4x4 proj = App->camera->GetProjection();
	float4x4 view = App->camera->GetView();
	float4x4 model = float4x4::identity;
	//float4x4 model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f), float4x4::RotateZ(pi / 4.0f), float3(2.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &model[0][0]); //GL_TRUE transpose the matrix
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &proj[0][0]);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(program, "mytexture"), 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}



bool ModuleRenderExercise::Init()
{
	vbo = CreateTriangleVBO();
	program = App->program->CreateProgramFromSource("HelloWorld.vert", "HelloWorld.frag");
	
	Texture2D texture2D = Texture2D();
	texture = texture2D.LoadTexture("Lenna.png");
	
	return true;
}

update_status ModuleRenderExercise::PreUpdate(float deltaTime)
{
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::Update(float deltaTime)
{
	RenderTriangle();
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::PostUpdate(float deltaTime)
{
	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
	DestroyVBO(vbo);
	return true;
}
