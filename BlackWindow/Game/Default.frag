#version 330 core
uniform vec3 my_color;
uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 viewPos;
uniform float ks,kd;
uniform vec3 colorAmbient;
uniform int N;
in vec2 uv;
uniform sampler2D texture_diffuse;
in vec3 fragment_normal;
in vec3 FragPos;
out vec4 color;

void main(){


	vec3 normal = normalize(fragment_normal);
	vec3 ambient = colorAmbient * texture(texture_diffuse, uv).rgb;
	//-----------------------------------

	vec3 lightDir = normalize(light_pos - FragPos);
	float nDtoL = max(dot(normal , - lightDir),0.0);
	vec3 diff = nDtoL * kd * light_color * texture(texture_diffuse, uv).rgb;
	//------------------------------------

	vec3 reflectDir = reflect(lightDir, normal);
	vec3 V = normalize(viewPos - FragPos);
	float spec = pow(max(dot(V , reflectDir), 0.0),N);
	vec3 specular = ks * spec * light_color;

	//-----------------------------------

	vec3 res = (ambient );
	color = vec4(res,1.0);

}
