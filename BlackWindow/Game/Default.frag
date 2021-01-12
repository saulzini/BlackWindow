#version 330 core
uniform vec3 my_color;
uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 viewPos;
uniform float ks,kd;
uniform vec3 colorAmbient;
uniform float N;
in vec2 uv;
//uniform sampler2D texture_diffuse;
//uniform sampler2D texture_Occlusion;
in vec3 fragment_normal;
in vec3 FragPos;
out vec4 color;


uniform struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
    
};

uniform struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

void main(){


	vec3 normal = normalize(fragment_normal);
	vec3 ambient = light.ambient * vec3(texture( material.diffuse, uv));
	//-----------------------------------

	vec3 lightDir = normalize( light_pos - FragPos);
	float nDtoL = max(dot(normal ,   lightDir),0.0);
	vec3 diff = nDtoL  * light.diffuse * vec3(texture(material.diffuse, uv));
	//------------------------------------

	vec3 reflectDir = reflect( -lightDir, normal);
	vec3 V = normalize(viewPos - FragPos);
	float spec = pow(max(dot(V ,  reflectDir), 0.0),material.shininess);
	vec3 specular =  spec * light.specular *  vec3(texture(material.specular,  uv));

	//-----------------------------------

	vec3 res = (ambient + diff + specular);
	color = vec4(res,1.0);
}
