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
