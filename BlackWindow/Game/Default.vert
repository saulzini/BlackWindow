#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform vec3 viewDire;

out vec3 FragPos;
out vec3 fragment_normal;
out vec2 uv;

void main()
{
     gl_Position = proj*view*model*vec4(position, 1.0);
     fragment_normal = (transpose(inverse(mat3(model)))) * Normal;
     FragPos = (model * vec4(position, 1.0)).xyz;
     uv = TexCoords;

}