#SHADER VERTEX
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;     
layout (location = 3) in vec2 aTexCoords;

out VS_OUT {
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
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gSpecular;

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


void main() {
    gPosition = fs_in.FragPos;
    gNormal   = normalize(fs_in.Normal);

    gAlbedo = texture(material.diffuse, fs_in.TexCoords).rgb;
    float depth = gl_FragCoord.z;

// Convert to linear depth

    // Visualize
    //gAlbedo = vec3(linearDepth);
    //gSpecular = texture(material.specular, fs_in.TexCoords).rgb;

    //gAlbedo = vec3(1.0, 0.0, 0.0);
    //gPosition = vec3(0.0);
    //gNormal = vec3(0.0, 0.0, 1.0);
    //gSpecular = vec3(0.0);

    //FragColor = vec3(texture(gAlbedo, fs_in.TexCoords), 1.0);


}

