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

struct Light {
    int   type; // 0 for directional light, 1 for point light
    vec3  pos;  // light's position in world coordinate system
    vec3  direction;
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
    vec3 ka, kd, ks;
    sampler2D ambTexture;
    sampler2D diffuseTexture;
    sampler2D specTexture;
};

in vec3 v_pos;
in vec3 v_normal;
in vec2 v_tex;

uniform vec3 u_viewPos;
uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_prjMat;

uniform mat4 u_normalMat;

uniform mat4 u_lightViewMat;
uniform mat4 u_lightPrjMat;

uniform Light    u_light;
uniform Material u_material;

uniform sampler2D u_depthTexture;

out vec4 o_color;

float computeDepth(vec3 pos);
float computeFade(Light light, float dist);

vec4  objDiffColor(Material material, vec2 texCoord);
vec4  objSpecColor(Material material, vec2 texCoord);
vec4  lightedSurface(Light light, Material material, vec3 pos, vec3 normal, vec2 texCoord, vec3 viewDir);
vec4  directionalLight(Light light, Material material, vec3 pos, vec3 normal, vec2 texCoord, vec3 viewDir);
vec4  pointLight(Light light, Material material, vec3 pos, vec3 normal, vec2 texCoord, vec3 viewDir);
float shadowCalculation(vec3 pos, vec3 normal, vec3 lightDir);

void main() {
    vec3 fragPos = vec3(u_modelMat * vec4(v_pos, 1.f));
    vec3 normal  = normalize(vec3(u_normalMat * vec4(v_normal, 0.f)));
    vec2 texCoord = v_tex;
    vec3 viewDir = normalize(u_viewPos - fragPos);

    o_color = lightedSurface(u_light, u_material, fragPos, normal, texCoord, viewDir);

    gl_FragDepth = computeDepth(v_pos);
}

float computeDepth(vec3 pos)
{
    vec4 posClipSpace = u_prjMat * u_viewMat * u_modelMat * vec4(pos, 1.f);
    return (posClipSpace.z / posClipSpace.w);
}

// attenuation for light's intensity/irradiance(power per area)
float computeFade(Light light, float dist)
{
    return 1.f / (light.attParamConst + light.attParamLinear * dist + light.attParamQuad * (dist * dist));
}

vec4 objDiffColor(Material material, vec2 texCoord)
{
    bool hasTexture = material.hasDiffTex != NO_TEXTURE;
    return hasTexture ? texture(material.diffuseTexture, texCoord) : vec4(material.kd, 1.f);
}

vec4 objSpecColor(Material material, vec2 texCoord)
{
    bool hasTexture = material.hasSpecTex != NO_TEXTURE;
    return hasTexture ? texture(material.specTexture, texCoord) : vec4(material.ks, 1.f);
}

vec4 lightedSurface(Light light, Material material, vec3 pos, vec3 normal, vec2 texCoord, vec3 viewDir)
{
    vec4 surface = vec4(0.f);

    if(light.type == LIGHT_TYPE_DIRECTIONAL)
    {
        surface = directionalLight(light, material, pos, normal, texCoord, viewDir);
    } 
    else if(light.type == LIGHT_TYPE_POINT)
    {
        surface = pointLight(light, material, pos, normal, texCoord, viewDir);
    }

    return surface;
}

vec4 directionalLight(Light light, Material material, vec3 pos, vec3 normal, vec2 texCoord, vec3 viewDir)
{
    vec4 objDiff = objDiffColor(material, texCoord);

    vec3 la = light.ka * light.ia; // light's ambient 
    vec3 ld = light.kd * light.id; // light's diffuse
    vec3 ls = vec3(0.f);           // light's specular

    vec3  lightDir = -normalize(light.direction);
    float diff = max(dot(normal, lightDir), 0.f);

    ld *= diff;

    // set object's diffuse color
    la *= objDiff.rgb;
    ld *= objDiff.rgb;

    if(material.colorMode == COLOR_MODE_ALL) {
        // blinn-phong mode
        vec3  halfwayDir = normalize(lightDir + viewDir);
        float specFactor = pow(max(dot(halfwayDir, normal), 0.f), light.shininess);
        vec4 objSpec = objSpecColor(material, texCoord);
        ls = light.ks * light.is * specFactor * objSpec.rgb;
    }

    float shadow = shadowCalculation(pos, normal, lightDir);

    vec3 colorRgb = la + (1.f - shadow) * (ld + ls);

    vec4 result = vec4(colorRgb, objDiff.a);

    return result;
}

vec4 pointLight(Light light, Material material, vec3 pos, vec3 normal, vec2 texCoord, vec3 viewDir)
{
    // TODO: to be implemented.
    return vec4(1.f, 0.f, 0.f, 1.f);
}

float shadowCalculation(vec3 pos, vec3 normal, vec3 lightDir)
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
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005); 
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
