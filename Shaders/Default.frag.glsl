#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec3 VertexColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D tex0;
uniform vec3 viewPos; // Camera pos

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

void main()
{	
	// Attribs
	vec4 texColor = texture(tex0, TexCoord);
	vec3 norm = normalize(Normal);

	//Ambient
	vec3 ambient = material.ambient * light.ambient;

	//Diffuse
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0); // Diffuse
	vec3 diffuse = material.diffuse * diff * light.diffuse;

	//Specular
	vec3 viewDir =	normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // Specular... Phong
	//vec3 halfWayDir = normalize(lightDir + viewDir);
	//float spec = pow(max(dot(viewDir, halfWayDir), 0.0), material.shininess); // Specular... Blinn-phong
	vec3 specular = material.specular * spec * light.specular;

	// Phong / Blinn-Phong
	vec3 result = (ambient + diffuse + specular) * texColor.rgb * VertexColor;

	FragColor = vec4(result, texColor.a);
	//FragColor = vec4(normalize(Normal) * 0.5 + 0.5, 1.0); // Face normals
	
	
}
