#version 460 core

in vec3 ourColor;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D tex0;
uniform vec3 lightColor;
uniform float ambientStrength;

void main()
{	
	vec4 texColor = texture(tex0, texCoord);
	vec3 ambient = ambientStrength * lightColor;
	vec3 result = texColor.rgb * ambient * ourColor;
	FragColor = vec4(result, texColor.a);
}
