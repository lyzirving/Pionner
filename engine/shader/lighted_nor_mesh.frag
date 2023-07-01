// specify the minimum version of opengl glsl, if current version is lower than this, the shader will not complie
#version 330 core

precision highp float;

const int LIGHT_TYPE_DIRECTIONAL = 0;
const int LIGHT_TYPE_POINT       = 1;

const int TEX_TYPE_DIFFUSE  = 1;
const int TEX_TYPE_SPECULAR = 2;

const int NO_TEXTURE = 0;

const int COLOR_MODE_DIFFUSE = 2;
const int COLOR_MODE_ALL     = 3;

const vec3 SPL_DIR[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

struct Light {
    int   type; // 0 for directional light, 1 for point light
    vec3  ka, kd, ks; // color of ambient, diffuse, specular
    float ia, id, is; // intensity of ambient, diffuse, specular
    float shininess;
    float attParamConst, attParamLinear, attParamQuad; // params of attenuation for point light
};

struct Material {
    int  colorMode;    // mode3 = ambient + diffuse + specular
    int  hasAmbTex;    // 0 for no texture
    int  hasDiffTex;   // 0 for no texture
    int  hasSpecTex;   // 0 for no texture
    int  hasNormal;    // 0 for no texture
    vec3 ka, kd, ks;
    sampler2D ambTexture;
    sampler2D diffuseTexture;
    sampler2D specTexture;
    sampler2D normTexture;
};

uniform mat4 u_viewMat;
uniform mat4 u_prjMat;

uniform mat4 u_lightViewMat;
uniform mat4 u_lightPrjMat;

uniform Light    u_light;
uniform Material u_material;

uniform vec3  u_viewPos;
uniform vec3  u_lightPos;

uniform float u_farPlane;

uniform sampler2D   u_depthTexture;
uniform samplerCube u_cubeDepthTexture;

in VS_OUT {
    vec3 fragPos;
    vec2 texCoords;
    vec3 tangentLightPos;
    vec3 tangentLightDir;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
} fs_in;

out vec4 o_color;

vec4  objDiffColor(vec2 texCoord);
vec4  objSpecColor(vec2 texCoord);

float directionLightShadow(vec3 pos, vec3 tangentNormal, vec3 tangentLightDir);
float pointLightShadow(vec3 pos, vec3 tangentNormal, vec3 tangentLightDir);
float pointLightShadowSoft(vec3 pos, vec3 tangentNormal, vec3 tangentLightDir);

vec4 lightedSurface(vec3 fragPos, vec2 texCoords, vec3 tangentLightPos, vec3 tangentLightDir, 
                    vec3 tangentViewPos, vec3 tangentFragPos);

float computeDepth(vec3 pos);
float computeFade(float dist);

void main() {
    o_color = lightedSurface(fs_in.fragPos, fs_in.texCoords, fs_in.tangentLightPos, fs_in.tangentLightDir,
                             fs_in.tangentViewPos, fs_in.tangentFragPos);

    gl_FragDepth = computeDepth(fs_in.fragPos);
}

float computeDepth(vec3 pos)
{
    vec4 posClipSpace = u_prjMat * u_viewMat * vec4(pos, 1.f);
    return (posClipSpace.z / posClipSpace.w);
}

// attenuation for light's intensity/irradiance(power per area)
float computeFade(float dist)
{
    return 1.f / (u_light.attParamConst + u_light.attParamLinear * dist + u_light.attParamQuad * (dist * dist));
}

vec4 objDiffColor(vec2 texCoord)
{
    bool hasTexture = u_material.hasDiffTex != NO_TEXTURE;
    return hasTexture ? texture(u_material.diffuseTexture, texCoord) : vec4(u_material.kd, 1.f);
}

vec4 objSpecColor(vec2 texCoord)
{
    bool hasTexture = u_material.hasSpecTex != NO_TEXTURE;
    return hasTexture ? texture(u_material.specTexture, texCoord) : vec4(u_material.ks, 1.f);
}

float directionLightShadow(vec3 pos, vec3 tangentNormal, vec3 tangentLightDir)
{
    vec4 lightSpacePos = u_lightPrjMat * u_lightViewMat * vec4(pos, 1.f);
    // LightPos ranges from [-1, 1]
    vec3 lightSpaceCoord = lightSpacePos.xyz / lightSpacePos.w;
    // Clamp to [0, 1]
    lightSpaceCoord = lightSpaceCoord * 0.5f + 0.5f;
    if(lightSpaceCoord.z > 1.f)
    {
        return 0.f;
    }   
    // Shadow bias: fix shadow acne
    float bias = max(0.05 * (1.0 - dot(tangentNormal, tangentLightDir)), 0.005); 
    float curDepth = lightSpaceCoord.z;

    vec2 texelSize = 1.0 / textureSize(u_depthTexture, 0);

    float shadow = 0.f;
    // Pcf: make soft shadow.
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(u_depthTexture, lightSpaceCoord.xy + vec2(x, y) * texelSize).r; 
            shadow += curDepth - bias > pcfDepth  ? 1.0 : 0.0; 
        }    
    } 
    shadow /= 9.f;
    return shadow;
}

float pointLightShadow(vec3 fragPos, vec3 tangentNormal, vec3 tangentLightDir)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - u_lightPos;
    // ise the fragment to light vector to sample from the depth map    
    float closestDepth = texture(u_cubeDepthTexture, fragToLight).r;
    // it is currently in linear range between [0,1], let's re-transform it back to original depth value
    closestDepth *= u_farPlane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // we use a much larger bias since depth is now in [near_plane, far_plane] range
    float bias = max(0.005f * (1.0 - dot(tangentNormal, tangentLightDir)), (0.005f)); 
       
    return currentDepth -  bias > closestDepth ? 1.0 : 0.0; 
}

float pointLightShadowSoft(vec3 fragPos, vec3 tangentNormal, vec3 tangentLightDir)
{
    vec3 fragToLight = fragPos - u_lightPos;       
    float currentDepth = length(fragToLight);

    float shadow = 0.f;
    float bias = max(0.005f * (1.0 - dot(tangentNormal, tangentLightDir)), (0.005f)); 
    int sampleCnt = 20;

    float viewDist = length(u_viewPos - fragPos);
    float diskRadius = (1.f + (viewDist / u_farPlane)) / 25.f;

    for(int i = 0; i < sampleCnt; ++i)
    {
        float closestDepth = texture(u_cubeDepthTexture, fragToLight + normalize(SPL_DIR[i]) * diskRadius).r;
        closestDepth *= u_farPlane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }

    shadow /= float(sampleCnt);
    return shadow;
}

vec4 lightedSurface(vec3 fragPos, vec2 texCoords, vec3 tangentLightPos, vec3 tangentLightDir, vec3 tangentViewPos, vec3 tangentFragPos)
{
    vec4 objDiff = objDiffColor(texCoords);

    // Obtain normal from normal map in range [0,1].
    // Obtained normal is in tangent space.
    vec3 normal = texture(u_material.normTexture, texCoords).rgb;
    // Transform normal vector to range [-1,1].
    normal = normalize(normal * 2.0 - 1.0);

    vec3 la = u_light.ka * u_light.ia; // light's ambient 
    vec3 ld = u_light.kd * u_light.id; // light's diffuse
    vec3 ls = vec3(0.f);               // light's specular

    vec3 lightDir;
    if(u_light.type == LIGHT_TYPE_POINT)
    {
        lightDir = normalize(tangentLightPos - tangentFragPos);
    }
    else
    {
        lightDir = -normalize(tangentLightDir);
    }

    float diff = max(dot(normal, lightDir), 0.f);
    ld *= diff * objDiff.rgb;
    la *= objDiff.rgb;

    if(u_material.colorMode == COLOR_MODE_ALL) {
        // blinn-phong mode
        vec3  viewDir = normalize(tangentViewPos - tangentFragPos);
        vec3  halfwayDir = normalize(lightDir + viewDir);
        float specFactor = pow(max(dot(halfwayDir, normal), 0.f), u_light.shininess);
        
        vec4  objSpec = objSpecColor(texCoords);

        ls = u_light.ks * u_light.is * specFactor * objSpec.rgb;
    }

    // calculate shadow factor
    float shadow = 0.f;
    vec3 colorRgb;
    if(u_light.type == LIGHT_TYPE_POINT)
    {
        shadow = pointLightShadowSoft(fragPos, normal, lightDir);
        float dist = length(tangentLightPos - tangentFragPos);

        colorRgb = (la + (1.f - shadow) * (ld + ls)) * computeFade(dist);
    }
    else
    {
        shadow = directionLightShadow(fragPos, normal, lightDir);
        colorRgb = la + (1.f - shadow) * (ld + ls);
    }

    return vec4(colorRgb, objDiff.a);
}
