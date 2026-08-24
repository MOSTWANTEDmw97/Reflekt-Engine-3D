#SHADER VERTEX
#version 460 core

layout (location = 0) in vec3 aPos;          // vertex position
layout (location = 4) in mat4 instanceModel; // instancing support (slots 4–7)

uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
    // Handle instanced vs non‑instanced models
    mat4 finalModel = instanceModel;
    if (gl_InstanceID == 0 && model != mat4(0.0)) {
        finalModel = model;
    }

    gl_Position = lightSpaceMatrix * finalModel * vec4(aPos, 1.0);
}

#SHADER FRAGMENT
#version 460 core

void main()
{
    
    //gl_FragDepth = gl_FragCoord.z;
}