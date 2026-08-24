#SHADER VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aPos;
    // remove translation from view so skybox stays centered
    mat4 rotView = mat4(mat3(view));
    vec4 pos = projection * rotView * vec4(aPos, 1.0);
    gl_Position = pos.xyww; // force depth to 1.0
}


#SHADER FRAGMENT
#version 330 core
in vec3 TexCoords;

/*
layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gAlbedo;
layout(location = 3) out vec3 gSpecular;
*/
out vec4 FragColor;

uniform samplerCube skybox;


void main()
{
    FragColor = texture(skybox, TexCoords);
    //FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}

/*
void main()
{
    // Skybox doesn’t have a real position in world space,
    // so you can either set it to far plane depth or zero.
    gPosition = vec3(0.0);

    // Normals aren’t meaningful for skybox, but you must write something.
    gNormal = vec3(1.0, 1.0, 1.0);

    // Actual cubemap color
    gAlbedo = texture(skybox, TexCoords).rgb;

    // No specular contribution
    gSpecular = vec3(100.0);
}*/
