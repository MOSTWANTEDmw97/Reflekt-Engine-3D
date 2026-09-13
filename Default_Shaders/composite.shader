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
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D opaqueTex; //Slot 0
uniform sampler2D backgroundTex; //Slot 1
uniform sampler2D transparentTex; //Slot 2

uniform sampler2D opaqueDepthTex; //Slot 3
uniform sampler2D transparentDepthTex; //Slot 4

float LinearizeDepth(float depth, float zNear, float zFar)
{
    return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}

void main()
{
    vec4 opaque = texture(opaqueTex, TexCoords);
    vec4 transparent = texture(transparentTex, TexCoords);
    vec4 background = texture(backgroundTex, TexCoords);
    

    //vec4 geometryColor = mix(transparent, opaque, transparent.a);
    vec4 color = mix(opaque, background, opaque.a);
    vec4 sceneColor = color * (1.0 - transparent.a) + transparent;
    //vec4 sceneColor = mix(color, transparent, transparent.a);

    FragColor = sceneColor;


}