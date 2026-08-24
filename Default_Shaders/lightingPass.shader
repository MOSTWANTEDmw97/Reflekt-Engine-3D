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

struct Light {
    vec3 position;
    vec3 color;
};
uniform Light light;

void main() {
    // Sample G‑Buffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal  = normalize(texture(gNormal, TexCoords).rgb);
    vec3 Albedo  = texture(gAlbedo, TexCoords).rgb;
    vec3 SpecCol = texture(gSpecular, TexCoords).rgb;

    // Ambient
    vec3 ambient = 0.1 * Albedo;

    // Diffuse
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * light.color * Albedo;

    // Specular (Blinn-Phong)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
    vec3 specular = spec * light.color * SpecCol;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
    //FragColor = vec4(texture(gAlbedo, TexCoords).rgb, 1.0);

    //FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}

