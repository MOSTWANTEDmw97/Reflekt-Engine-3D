#version 460 core

out vec4 FragColor;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;

uniform vec3 lightColor;

void main()
{
	vec3 result = ambient * diffuse * specular;
	FragColor = vec4(vec3(1.0), 1.0);
}