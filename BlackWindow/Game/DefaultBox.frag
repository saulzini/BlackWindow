
#version 330 core
out vec4 FragColor;

in vec3 texcoords;

uniform samplerCube Skybox;

void main()
{    
    FragColor = texture(Skybox, texcoords);
}