#SHADER VERTEX
#version 460 core

layout (location = 0) in vec3 aPos; // vert position
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor; // vert color
layout (location = 3) in vec2 aTexCoord; // uv

out vec3 FragPos; //World space pos
out vec3 Normal; // World space normal dir
out vec3 VertexColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal;
	
	VertexColor = aColor;
	TexCoord = aTexCoord;

	gl_Position = projection * view * vec4(FragPos, 1.0);
}


#SHADER FRAGMENT
#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec3 VertexColor;
in vec2 TexCoord;

out vec4 FragColor;

//Uniforms
uniform vec3 viewPos; // Camera pos

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material material;


struct DirectionalLight {
	// W unused for now
    vec4 direction; 
    vec4 ambient;   
    vec4 diffuse;   
    vec4 specular;  
};
#define NR_DIR_LIGHT 2
//uniform DirectionalLight directionalLights[NR_DIR_LIGHT];
layout(std430, binding = 0) buffer DirectionalLightBuffer
{
	DirectionalLight directionalLights[];
};

struct PointLight {
    vec4 position; 
    vec4 ambient_And_Constant;   
    vec4 diffuse_And_Linear;   
    vec4 specular_And_Quadratic;
};
#define NR_POINT_LIGHT 4
//uniform PointLight pointLights[NR_POINT_LIGHT];
layout(std430, binding = 1) buffer PointLightBuffer
{
	PointLight pointLights[];
};
struct SpotLight {
    vec3 position_And_Cutoff; 
    vec3 direction_And_OuterCutoff; 
    vec4 ambient_And_Constant;  
    vec4 diffuse_And_Linear;   
    vec4 specular_And_Quadratic;
};
#define NR_SPOT_LIGHT 2
//uniform SpotLight spotLights[NR_SPOT_LIGHT];
layout(std430, binding = 2) buffer SpotLightBuffer
{
	SpotLight spotLights[];
};

//Function declarations
float LinearizeDepth(float depth, float near, float far);

vec3 CalcDirLight(DirectionalLight light, vec3 norm, vec3 viewDir);
vec3 CalcPointLight(PointLight pointLight, vec3 norm, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight spotLight, vec3 norm, vec3 fragPos, vec3 viewDir);

void main()
{	
	// Attribs
	vec3 norm = normalize(Normal);
	vec3 viewDir =	normalize(viewPos - FragPos);

	vec3 result = vec3(0.0);

	for(int i = 0; i < directionalLights.length(); i++)
	{
		result += CalcDirLight(directionalLights[i], norm, viewDir);
	}
	for(int i = 0; i < pointLights.length(); i++)
	{
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}
	for(int i = 0; i < spotLights.length(); i++)
	{
		result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
	}
	

	vec3 litColor = result;
	
	// Final color with alpha preserved
	vec4 texColor = texture(material.diffuse, TexCoord);
	vec4 sceneColor = vec4(litColor, texColor.a);

	if(sceneColor.a < 0.25)
		discard;

	FragColor = sceneColor;	
	//FragColor = vec4(normalize(Normal) * 0.5 + 0.5, 1.0); // Face normals
	
}

float LinearizeDepth(float depth, float near, float far)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

vec3 CalcDirLight(DirectionalLight light, vec3 norm, vec3 viewDir)
{
	// Note it is the dir from the light source towards others...
    vec3 lightDir = normalize(-light.direction.xyz); 
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	
    vec3 texDiffuse = vec3(texture(material.diffuse, TexCoord));
    vec3 texSpecular = texture(material.specular, TexCoord).rgb;

    vec3 ambient  = light.ambient.xyz  * texDiffuse;
    vec3 diffuse  = light.diffuse.xyz  * diff * texDiffuse;
    vec3 specular = light.specular.xyz * spec * texSpecular;

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position.xyz - FragPos);

	//Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
	//Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	//Attenuation
	float distance = length(light.position.xyz - FragPos);
	float attenuation = 1.0;
	if(light.ambient_And_Constant.w != 0 || light.diffuse_And_Linear.w != 0 || light.specular_And_Quadratic.w != 0)
		attenuation = 1.0 / (light.ambient_And_Constant.w + (light.diffuse_And_Linear.w * distance) + (light.specular_And_Quadratic.w * (distance * distance)));

    vec3 texDiffuse = vec3(texture(material.diffuse, TexCoord));
    vec3 texSpecular = texture(material.specular, TexCoord).rgb;
	//Combine
    vec3 ambient  = light.ambient_And_Constant.xyz  * texDiffuse;
    vec3 diffuse  = light.diffuse_And_Linear.xyz  * diff * texDiffuse;
    vec3 specular = light.specular_And_Quadratic.xyz * spec * texSpecular;

	return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position_And_Cutoff.xyz - fragPos);
		
	//Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
	//Specular
    vec3 reflectDir = reflect(lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	//Attenuation
	float distance = length(-light.position_And_Cutoff.xyz - FragPos);
	float attenuation = 1.0;
	if(light.ambient_And_Constant.w != 0 || light.diffuse_And_Linear.w != 0 || light.specular_And_Quadratic.w != 0)
		attenuation = 1.0 / (light.ambient_And_Constant.w + (light.diffuse_And_Linear.w * distance) + (light.specular_And_Quadratic.w * (distance * distance)));

	//Spot light intensity (smooth edges)
	float theta = dot(lightDir, normalize(-light.direction_And_OuterCutoff));
	float epsilon = light.position_And_Cutoff.z - light.direction_And_OuterCutoff.z;
	float intensity = 0.0;
	if (epsilon != 0.0)
		intensity = clamp((theta - light.direction_And_OuterCutoff.z) / epsilon, 0.0, 1.0);

	//float intensity = smoothstep(0.0, 1.0, (theta - light.outerCutoff) / epsilon);
	
    vec3 texDiffuse = vec3(texture(material.diffuse, TexCoord));
    vec3 texSpecular = texture(material.specular, TexCoord).rgb;
	//Combine
    vec3 ambient  = light.ambient_And_Constant.xyz  * texDiffuse;
    vec3 diffuse  = light.diffuse_And_Linear.xyz  * diff * texDiffuse;
    vec3 specular = light.specular_And_Quadratic.xyz * spec * texSpecular;

	return (ambient + (diffuse + specular) * intensity) * attenuation;

}
