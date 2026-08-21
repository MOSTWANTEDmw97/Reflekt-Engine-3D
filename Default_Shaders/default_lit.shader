#SHADER VERTEX
#version 460 core

layout (location = 0) in vec3 aPos; // vert position
layout (location = 1) in vec3 aNormal; //vert normal dir
layout (location = 2) in vec3 aColor; // vert color
layout (location = 3) in vec2 aTexCoord; // uv


// Instancing: mat4 takes up 4 attribute slots (4–7)
layout (location = 4) in mat4 instanceModel;

out vec3 vFragPos; //World space pos
out vec3 vNormal; // World space normal dir
out vec3 vVertexColor;
out vec2 vTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    //Safeguard for supporting instanced and non instanced models
    mat4 finalModel = instanceModel;
    if (gl_InstanceID == 0 && model != mat4(0.0)) 
    {
        finalModel = model;
    }
    
	vFragPos = vec3(finalModel * vec4(aPos, 1.0));
	vNormal = mat3(transpose(inverse(model))) * aNormal;
	
	vVertexColor = aColor;
	vTexCoord = aTexCoord;

	gl_Position = projection * view * vec4(vFragPos, 1.0);
}


#SHADER GEOMETRY
#version 460 core

layout(triangles) in;                               // input primitive type
layout(triangle_strip, max_vertices = 3) out;       // output type

in vec3 vFragPos[];      // from vertex shader (array because multiple verts per primitive)
in vec3 vNormal[];       
in vec3 vVertexColor[];
in vec2 vTexCoord[];

out vec3 FragPos;      // pass to fragment shader
out vec3 Normal;
out vec3 VertexColor;
out vec2 TexCoord;

uniform float time;

void main()
{
    
    // Compute a face normal (average of the three vertex normals)
    vec3 faceNormal = normalize(vNormal[0] + vNormal[1] + vNormal[2]);

    // Explosion factor oscillates with time
    float explode = sin(time) * 0.5; // tweak multiplier for strength

    for (int i = 0; i < 3; i++)
    {
        // Push each vertex outward along the face normal
        //vec4 displacedPos = gl_in[i].gl_Position + vec4(faceNormal * explode, 0.0);

        FragPos     = vFragPos[i] + faceNormal * explode;
        Normal      = vNormal[i];
        VertexColor = vVertexColor[i];
        TexCoord    = vTexCoord[i];
        gl_Position = gl_in[i].gl_Position;

        EmitVertex();
    }
    EndPrimitive();
}


#SHADER FRAGMENT
#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec3 VertexColor;
in vec2 TexCoord;

out vec4 FragColor;

//Uniforms
uniform vec3 viewPos; // Camera pos

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material material;

struct Light {
    int type;            // stored in .x of a vec4
    vec4 position;       // xyz = position, w unused
    vec4 direction;      // xyz = direction, w unused
    vec4 color;          // rgb = color, a = intensity
    vec4 attenuation;    // x = constant, y = linear, z = quadratic, w unused
    vec4 cutoff;         // x = innerCutoff, y = outerCutoff, z/w unused
};
layout(std430, binding = 0) buffer LightBuffer {
    Light lights[];
};

//Function declarations

vec3 CalcLight(Light light, vec3 norm, vec3 fragPos, vec3 viewDir) ;

void main()
{	
	// Attribs
	vec3 norm = normalize(Normal);
	vec3 viewDir =	normalize(viewPos - FragPos);

	vec3 result = vec3(0.0);
    for (int i = 0; i < lights.length(); i++)
    {
        result += CalcLight(lights[i], norm, FragPos, viewDir);
    }
	

	vec3 litColor = result;
	
	// Final color with alpha preserved
	vec4 texColor = texture(material.diffuse, TexCoord);
	vec4 sceneColor = vec4(litColor, texColor.a);

	if(sceneColor.a < 0.25)
		discard;

	FragColor = sceneColor;	
	//FragColor = vec4(normalize(Normal) * 0.5 + 0.5, 1.0); // Face normals
	
}


vec3 CalcLight(Light light, vec3 norm, vec3 fragPos, vec3 viewDir) {
    vec3 texDiffuse = texture(material.diffuse, TexCoord).rgb;
    vec3 texSpecular = texture(material.specular, TexCoord).rgb;

    vec3 lightDir;
    float attenuation = 1.0;
    float spotIntensity = 1.0;

    if (light.type == 0) 
	{
        lightDir = normalize(-light.direction.xyz);
    } 
	else 
	{
		//Light Type 1
        lightDir = normalize(light.position.xyz - fragPos);
        float distance = length(light.position.xyz - fragPos);
        attenuation = 1.0 / (light.attenuation.x +
                             light.attenuation.y * distance +
                             light.attenuation.z * distance * distance);

        if (light.type == 2) 
		{
            float theta = dot(lightDir, normalize(-light.direction.xyz));
            float epsilon = light.cutoff.x - light.cutoff.y;
            spotIntensity = clamp((theta - light.cutoff.y) / epsilon, 0.0, 1.0);
        }
    }

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient  = light.color.rgb * texDiffuse * 0.1;
    vec3 diffuse  = light.color.rgb * diff * texDiffuse;
    vec3 specular = light.color.rgb * spec * texSpecular;

    return (ambient + (diffuse + specular) * spotIntensity) * attenuation * light.color.a;
}
