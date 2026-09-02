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

uniform sampler2D opaqueTex;      // DeferredLitBuffer.colorTex
uniform sampler2D backgroundTex;
uniform sampler2D transparentTex; // TransparentBuffer.colorTex



void main()
{
    vec4 opaque = texture(opaqueTex, TexCoords);

    vec4 transparent = texture(transparentTex, TexCoords);
    vec4 background = texture(backgroundTex, TexCoords);

    // Blend transparents over opaque
    //vec4 geometryColor = mix(transparent, opaque, transparent.a);
    vec4 color = mix(opaque, background, opaque.a);
    vec4 sceneColor = color * (1.0 - transparent.a) + transparent;
    //vec4 sceneColor = mix(color, transparent, transparent.a);

    //vec4 sceneColor = geometryColor + transparent;
    //vec4 sceneColor = opaque * transparent;
    //vec4 sceneColor = vec4(opaque.rgb * transparent.rgb, 1.0);
    //vec4 sceneColor = vec4(mix(opaque.rgb, transparent.rgb, transparent.a), 1.0);
    float depth = opaque.r;
    
    float zNear = 0.1;
    float zFar  = 1000.0;
    float linearDepth = (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));

    FragColor = sceneColor;


}