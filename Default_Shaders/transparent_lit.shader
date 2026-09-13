#SHADER VERTEX
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;     
layout (location = 3) in vec2 aTexCoords;

out VS_OUT 
{
    vec3 FragPos;
    vec3 Normal;
    vec3 Color;
    vec2 TexCoords;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal  = mat3(transpose(inverse(model))) * aNormal;
    vs_out.Color   = aColor;
    vs_out.TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(vs_out.FragPos, 1.0);
}

#SHADER FRAGMENT
#version 460 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec3 Color;
    vec2 TexCoords;
} fs_in;


struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material material;


uniform vec3 viewPos;


//Light SSBO
struct Light
{
    ivec4 type;
    vec4 position;
    vec4 direction;
    vec4 ambientAndIntensity;
    vec4 diffuse;
    vec4 specular;
    vec4 attenuation;
    vec4 cutoff;
};
layout(std430, binding = 0) buffer LightBuffer
{
    Light lights[];
};

uniform sampler2D opaqueDepthTex; //Slot 3


void CalcDirLight(Light light, vec3 norm, vec3 fragPos, vec3 viewDir, out vec3 result);
void CalcPointLight(Light light, vec3 norm, vec3 fragPos, vec3 viewDir, out vec3 result);
void CalcSpotLight(Light light, vec3 norm, vec3 fragPos, vec3 viewDir, out vec3 result);

float LinearizeDepth(float depth, float zNear, float zFar);

void main()
{
    float zNear = 0.1;
    float zFar = 1000.0;

    vec4 texColor = texture(material.diffuse, fs_in.TexCoords);
    vec3 albedo   = texColor.rgb;
    float alpha   = texColor.a; // transparency


    vec2 screenUV = gl_FragCoord.xy / vec2(textureSize(opaqueDepthTex, 0));
    float fragDepth = LinearizeDepth(gl_FragCoord.z, zNear, zFar);
    float opaqueDepth = LinearizeDepth(texture(opaqueDepthTex, screenUV).r, zNear, zFar);
    
    if(fragDepth > opaqueDepth + 1e-5) // If the fragment is behind the opaque geometry, discard it
    {
        discard;
    }
    vec3 result = vec3(0.0);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    for(int i = 0; i < lights.length(); ++i)
    {
        vec3 lightResult;

        if (lights[i].type.x == 0) // Directional light
            CalcDirLight(lights[i], fs_in.Normal, fs_in.FragPos, viewDir, lightResult);
        else if (lights[i].type.x == 1) // Point light
            CalcPointLight(lights[i], fs_in.Normal, fs_in.FragPos, viewDir, lightResult);
        else if (lights[i].type.x == 2) // Spot light
            CalcSpotLight(lights[i], fs_in.Normal, fs_in.FragPos, viewDir, lightResult);

        result += lightResult;
    }
    
    FragColor = vec4(result, alpha);
    //FragColor = vec4(albedo, alpha);

}

void CalcDirLight(Light light, vec3 norm, vec3 fragPos, vec3 viewDir, out vec3 result)
{
    vec3 albedo   = texture(material.diffuse, fs_in.TexCoords).rgb;
    vec3 specCol  = texture(material.specular, fs_in.TexCoords).rgb;
    float shininess = texture(material.specular, fs_in.TexCoords).a;
    
    // Ambient
    vec3 ambient = light.ambientAndIntensity.rgb * albedo;

    // Diffuse
    vec3 lightDir = normalize(-light.direction.xyz);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse.rgb * albedo;

    // Specular (Blinn-Phong)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specular = spec * light.specular.rgb * specCol;
    //specular *= light.ambientAndIntensity.a;
    result = (ambient + diffuse + specular) * light.ambientAndIntensity.a;
    
}

void CalcPointLight(Light light, vec3 norm, vec3 fragPos, vec3 viewDir, out vec3 result)
{
    vec3 albedo   = texture(material.diffuse, fs_in.TexCoords).rgb;
    vec3 specCol  = texture(material.specular, fs_in.TexCoords).rgb;
    float shininess = texture(material.specular, fs_in.TexCoords).a;

    // Ambient
    vec3 ambient = (light.ambientAndIntensity.rgb * albedo);

    // Diffuse
    vec3 lightDir = normalize(light.position.xyz - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse.rgb * albedo;

    // Specular (Blinn-Phong)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specular = spec * light.specular.rgb * specCol;

    // Attenuation
    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (light.attenuation.x +
                               light.attenuation.y * distance +
                               light.attenuation.z * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    result = (ambient + diffuse + specular) * light.ambientAndIntensity.a;
}

void CalcSpotLight(Light light, vec3 norm, vec3 fragPos, vec3 viewDir, out vec3 result)
{
    vec3 albedo   = texture(material.diffuse, fs_in.TexCoords).rgb;
    vec3 specCol  = texture(material.specular, fs_in.TexCoords).rgb;
    float shininess = texture(material.specular, fs_in.TexCoords).a;

    // Ambient
    vec3 ambient = light.ambientAndIntensity.rgb * albedo;

    // Diffuse
    vec3 lightDir = normalize(light.position.xyz - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse.rgb * albedo;

    // Specular (Blinn-Phong)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specular = spec * light.specular.rgb * specCol;

    // Attenuation
    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (light.attenuation.x +
                               light.attenuation.y * distance +
                               light.attenuation.z * (distance * distance));

    // Spotlight intensity (use cos for cutoff angles)
    float theta = dot(lightDir, normalize(-light.direction.xyz));
    float epsilon = cos(light.cutoff.x) - cos(light.cutoff.y);
    float intensity = clamp((theta - cos(light.cutoff.y)) / epsilon, 0.0, 1.0);

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    
    result = (ambient + diffuse + specular) * light.ambientAndIntensity.a;
}


float LinearizeDepth(float depth, float zNear, float zFar)
{
    return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}