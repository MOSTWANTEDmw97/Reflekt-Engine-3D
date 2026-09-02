#SHADER VERTEX
#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    gl_Position = projection * mat4(mat3(view)) * vec4(aPos, 1.0);
}


#SHADER FRAGMENT
#version 460 core
in vec3 TexCoords;

out vec4 FragColor;

uniform samplerCube skybox;

// --- Worley-style cellular noise ---


// Hash returning vec2
vec2 hash(vec2 p) {
    float h = fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
    float k = fract(sin(dot(p, vec2(269.5, 183.3))) * 43758.5453123);
    return vec2(h, k);
}

float worley(vec2 uv) {
    vec2 i = floor(uv);
    vec2 f = fract(uv);
    float d = 1.0;
    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            vec2 neighbor = vec2(x, y);
            vec2 point = hash(i + neighbor); // pseudo-random offset
            vec2 diff = neighbor + point - f;
            d = min(d, length(diff));
        }
    }
    return d;
}

float hashi(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}

// 2D value noise
float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);

    float a = hashi(i);
    float b = hashi(i + vec2(1.0, 0.0));
    float c = hashi(i + vec2(0.0, 1.0));
    float d = hashi(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) +
           (c - a) * u.y * (1.0 - u.x) +
           (d - b) * u.x * u.y;
}

vec3 Cloud();
void main()
{
    FragColor = texture(skybox, vec3(TexCoords.x, TexCoords.y, TexCoords.z));
 

    //FragColor = vec4(vec3(linearDepth), 1.0);
    vec3 finalColor = Cloud();
    //FragColor = vec4(finalColor, 1.0);
}

vec3 Cloud()
{
    
    vec3 dir = normalize(TexCoords);

    // --- Sky gradient: blue → pink ---
    float t = dir.y * 0.8 + 0.1;
    vec3 topColor    = vec3(0.2, 0.5, 0.9); // deep sky blue
    vec3 horizonColor= vec3(1.0, 0.6, 0.8); // soft pink
    vec3 baseSky     = mix(horizonColor, topColor, t);

    // --- Clouds (Worley blobs) ---
    vec2 uv = dir.xz * 10.0;
    float cell = worley(uv);
    float clouds = step(0.8, 1.0 - cell);
    vec3 cloudColor = vec3(1.0);
    vec3 skyWithClouds = mix(baseSky, cloudColor, clouds);

    // --- Hills silhouette with detail ---
    // Base rolling hills
    float hillLine = 0.2 
                   + 0.05 * sin(uv.x * 3.0) 
                   + 0.05 * cos(uv.y * 2.0);

    // Add irregularity with noise
    float hillNoise = noise(uv * 0.5) * 0.05;
    hillLine += hillNoise;

    // Mask: below hillLine → hills
    float hillMask = step(dir.y, hillLine);

    // Hill color with vertical gradient (dark base → lighter top)
    float hillShade = smoothstep(0.0, hillLine, dir.y);
    vec3 hillBaseColor = vec3(0.1, 0.2, 0.1); // dark green
    vec3 hillTopColor  = vec3(0.2, 0.7, 0.2); // lighter green
    vec3 hillColor     = mix(hillBaseColor, hillTopColor, hillShade);

    // Blend hills into sky
    vec3 finalColor = mix(skyWithClouds, hillColor, hillMask);
    return finalColor;
}