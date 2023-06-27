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

uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_prjMat;

uniform mat3 u_normalMat;

uniform vec3 u_viewPos;
uniform vec3 u_lightPos;
uniform vec3 u_lightDir;

uniform mat4 u_lightViewMat;
uniform mat4 u_lightPrjMat;

uniform Light    u_light;
uniform Material u_material;

uniform sampler2D u_depthTexture;

out vec4 o_color;

float computeDepth(vec3 pos);
float computeFade(Light light, float dist);

vec4  objDiffColor(vec2 texCoord);
vec4  objSpecColor(vec2 texCoord);
float shadowCalculation(vec3 pos, vec3 normal, vec3 lightDir);

vec4  directionalLight(vec3 fragPos, vec2 texCoords, vec3 normal);

void main() {
    vec3 fragPos = vec3(u_modelMat * vec4(v_pos, 1.f));
    vec2 texCoord = v_tex;
    vec3 normal  = normalize(u_normalMat * v_normal);
    vec3 viewDir = normalize(u_viewPos - fragPos);

    vec4 surface = vec4(0.f, 0.f, 0.f, 1.f);

    if(u_light.type == LIGHT_TYPE_DIRECTIONAL)
    {
        surface = directionalLight(fragPos, texCoord, normal);
    }

    o_color = surface;

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

vec4 directionalLight(vec3 fragPos, vec2 texCoords, vec3 normal)
{
    vec4 objDiff = objDiffColor(texCoords);

    vec3 la = u_light.ka * u_light.ia; // light's ambient 
    vec3 ld = u_light.kd * u_light.id; // light's diffuse
    vec3 ls = vec3(0.f);           // light's specular

    vec3  lightDir = -normalize(u_lightDir);
    float diff = max(dot(normal, lightDir), 0.f);

    ld *= diff;

    // set object's diffuse color
    la *= objDiff.rgb;
    ld *= objDiff.rgb;

    if(u_material.colorMode == COLOR_MODE_ALL) {
        // blinn-phong mode
        vec3  viewDir = normalize(u_viewPos - fragPos);
        vec3  halfwayDir = normalize(lightDir + viewDir);
        float specFactor = pow(max(dot(halfwayDir, normal), 0.f), u_light.shininess);
        vec4  objSpec = objSpecColor(texCoords);

        ls = u_light.ks * u_light.is * specFactor * objSpec.rgb;
    }

    float shadow = shadowCalculation(fragPos, normal, lightDir);

    vec3 colorRgb = la + (1.f - shadow) * (ld + ls);

    return vec4(colorRgb, objDiff.a);
}
