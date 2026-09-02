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

struct Light {
    vec3 position;
    vec3 color;
};
uniform Light light;

void main()
{
    vec4 texColor = texture(material.diffuse, fs_in.TexCoords);
    vec3 albedo   = texColor.rgb;
    float alpha   = texColor.a; // transparency

    // Ambient
    vec3 ambient = 0.1 * albedo;

    // Diffuse
    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color * albedo;

    // Specular (Blinn-Phong)
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 16.0);
    vec3 specular = spec * light.color;

    vec3 lighting = ambient + diffuse + specular;

    //FragColor = vec4(albedo.r, albedo.g, 0.0, 0.5);
    FragColor = vec4(lighting, alpha);
    //FragColor = vec4(1.0, 0.0, 0.0, 0.1);
    //FragColor = texture(material.diffuse, fs_in.TexCoords);

}
