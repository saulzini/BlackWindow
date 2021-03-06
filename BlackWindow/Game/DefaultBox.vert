#version 440

uniform mat4 view;
uniform mat4 proj;

layout (location = 0) in vec3 pos;
out vec3 texcoords;

void main() 
{
   texcoords = vec3(-pos.x, -pos.yz);
   vec4 p = proj*vec4(mat3(view)* pos, 1.0);
   gl_Position = p.xyww;
}
