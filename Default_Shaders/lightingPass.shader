#SHADER VERTEX
#version 460 core
layout (location = 0) in vec2 aPos;       // ScreenQuad
layout (location = 1) in vec2 aTexCoords; // ScreenQuad

out vec2 TexCoords;

void main() {
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 0.0, 1.0);
}

#SHADER FRAGMENT
#version 460 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpecular;

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



void CalcDirLight(Light light, vec3 norm, vec3 fragPos, vec3 viewDir, out vec3 result);
void CalcPointLight(Light light, vec3 norm, vec3 fragPos, vec3 viewDir, out vec3 result);
void CalcSpotLight(Light light, vec3 norm, vec3 fragPos, vec3 viewDir, out vec3 result);

void main() {
    // Sample G‑Buffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal  = normalize(texture(gNormal, TexCoords).rgb);
    vec3 Albedo  = texture(gAlbedo, TexCoords).rgb;
    vec3 SpecCol = texture(gSpecular, TexCoords).rgb;
    float alpha = texture(gAlbedo, TexCoords).a;
    

    //Lighting calculations
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0);
    for(int i = 0; i < lights.length(); ++i)
    {
        vec3 lightResult;

        if (lights[i].type.x == 0) // Directional light
            CalcDirLight(lights[i], Normal, FragPos, viewDir, lightResult);
        else if (lights[i].type.x == 1) // Point light
            CalcPointLight(lights[i], Normal, FragPos, viewDir, lightResult);
        else if (lights[i].type.x == 2) // Spot light
            CalcSpotLight(lights[i], Normal, FragPos, viewDir, lightResult);

        result += lightResult;
    }
    
    FragColor = vec4(result, alpha);
    //FragColor = vec4(normalize(viewPos - FragPos) * 0.5 + 0.5, 0.0);

    //FragColor = vec4(texture(gAlbedo, TexCoords));

}

void CalcDirLight(Light light, vec3 norm, vec3 fragPos, vec3 viewDir, out vec3 result)
{
    vec3 albedo   = texture(gAlbedo, TexCoords).rgb;
    vec3 specCol  = texture(gSpecular, TexCoords).rgb;
    float shininess = texture(gSpecular, TexCoords).a;
    
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
    vec3 albedo   = texture(gAlbedo, TexCoords).rgb;
    vec3 specCol  = texture(gSpecular, TexCoords).rgb;
    float shininess = texture(gSpecular, TexCoords).a;

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
    vec3 albedo   = texture(gAlbedo, TexCoords).rgb;
    vec3 specCol  = texture(gSpecular, TexCoords).rgb;
    float shininess = texture(gSpecular, TexCoords).a;

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


