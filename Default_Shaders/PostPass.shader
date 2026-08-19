#SHADER VERTEX
#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
	TexCoord = aTexCoord;
	gl_Position = vec4(aPos, 0.0, 1.0);
}


#SHADER FRAGMENT
#version 460 core

in vec2 TexCoord;
out vec4 FragColor;
  
uniform sampler2D screenTex;

void main()
{ 
	FragColor = texture(screenTex , TexCoord);
	
	
    //float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;

	//FragColor = vec4(average, average, average, 1.0f);

    //FragColor = vec4(vec3(1.0 - texture(screenTex, TexCoord)), 1.0);
    //FragColor = texture(screenTex, TexCoord);
	//FragColor = vec4(TexCoord, 0.0f, 1.0f);
}