#version 460 core

in vec3 ourColor;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D tex0;

void main()
{
	vec4 texColor = texture(tex0, texCoord);
	
	FragColor = texColor * vec4(ourColor, 1.0);
}
