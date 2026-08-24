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
	vNormal = mat3(transpose(inverse(finalModel))) * aNormal;
	
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
uniform sampler2D shadowMap;

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
    vec4 ambient; // RGB col, a intensity
    vec4 diffuse;
    vec4 specular;
    vec4 attenuation;    // x = constant, y = linear, z = quadratic, w unused
    vec4 cutoff;         // x = innerCutoff, y = outerCutoff, z/w unused
};
layout(std430, binding = 0) buffer LightBuffer {
    Light lights[];
};


struct LightResult {
    vec3 ambient;
    vec3 diffuseSpec;
};

//Function declarations

LightResult CalcLight(Light light, vec3 norm, vec3 fragPos, vec3 viewDir);


void main()
{	
	// Attribs
	vec3 norm = normalize(Normal);
	vec3 viewDir =	normalize(viewPos - FragPos);

	vec3 result = vec3(0.0);

    vec3 ambientSum = vec3(0.0);
    vec3 diffSpecSum = vec3(0.0);
    
    LightResult lr;
    for (int i = 0; i < lights.length(); i++) {
        lr = CalcLight(lights[i], norm, FragPos, viewDir);
        //ambientSum += lr.ambient;
        //diffSpecSum += lr.diffuseSpec;
    }
	
    //float shadow = CalcShadow(FragPosLightSpace, norm, normalize(lights[0].direction.xyz - FragPos));
    vec3 litColor = lr.ambient + (1.0) * lr.diffuseSpec;
    //vec3 litColor = lr.ambient + (1.0) * lr.diffuseSpec;
    //float shadow = ShadowCalculation(FragPosLightSpace);
    //vec3 litColor = result * (1.0 - shadow);


	
	// Final color with alpha preserved
	vec4 texColor = texture(material.diffuse, TexCoord);
	vec4 sceneColor = vec4(litColor, texColor.a);

	if(sceneColor.a < 0.25)
		discard;

	FragColor = sceneColor;	
	//FragColor = vec4(normalize(Normal) * 0.5 + 0.5, 1.0); // Face normals
	
}

LightResult CalcLight(Light light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
    vec3 texDiffuse  = texture(material.diffuse, TexCoord).rgb;
    vec3 texSpecular = texture(material.specular, TexCoord).rgb;

    float attenuation = 1.0;
    float spotIntensity = 1.0;

    if (light.type == 0) {
        // Directional light
        
        vec3 lightDir;
        lightDir = normalize(-light.direction.xyz);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

        vec3 ambient  = light.ambient.rgb  * texDiffuse;
        vec3 diffuse  = light.diffuse.rgb  * diff * texDiffuse;
        vec3 specular = light.specular.rgb * spec * texSpecular;

        
        LightResult lr;
        lr.ambient = ambient;
        lr.diffuseSpec = (diffuse + specular);
        return lr;

    }
    else if (light.type == 1) {
        // Point light
        
        vec3 lightDir;
        lightDir = normalize(light.position.xyz - fragPos);
        float distance = length(light.position.xyz - fragPos);
        attenuation = 1.0 / (light.attenuation.x +
                             light.attenuation.y * distance +
                             light.attenuation.z * distance * distance);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

        vec3 ambient  = light.ambient.rgb  * texDiffuse;
        vec3 diffuse  = light.diffuse.rgb  * diff * texDiffuse;
        vec3 specular = light.specular.rgb * spec * texSpecular;
        
        LightResult lr;
        lr.ambient = ambient * attenuation;
        lr.diffuseSpec = (diffuse + specular) * attenuation;
        return lr;

    }
    else if (light.type == 2) {
        // Spotlight
        
        vec3 lightDir;
        lightDir = normalize(light.position.xyz - fragPos);
        float distance = length(light.position.xyz - fragPos);
        attenuation = 1.0 / (light.attenuation.x +
                             light.attenuation.y * distance +
                             light.attenuation.z * distance * distance);

        float theta = dot(lightDir, normalize(-light.direction.xyz));
        float epsilon = light.cutoff.x - light.cutoff.y;
        spotIntensity = clamp((theta - light.cutoff.y) / epsilon, 0.0, 1.0);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

        vec3 ambient  = light.ambient.rgb  * texDiffuse;
        vec3 diffuse  = light.diffuse.rgb  * diff * texDiffuse;
        vec3 specular = light.specular.rgb * spec * texSpecular;
        
        LightResult lr;
        lr.ambient = ambient * attenuation;
        lr.diffuseSpec = (diffuse + specular) * spotIntensity * attenuation;
        return lr;
    }

    // Default return if type is invalid
    return LightResult(vec3(0.0), vec3(0.0));
}

