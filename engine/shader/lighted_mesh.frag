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
    int  texType;      // 1 for diffuse, 2 for specular
    int  hasTexture;   // 0 for no texture, otherwise this mesh has one texture
    vec3 ka, kd, ks;
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
float computeAttenuation(Light light, float dist);

vec4  materialColor(Material material, vec2 texCoord);
vec4  lightedSurface(Light light, Material material, vec3 pos, vec3 normal, vec2 texCoord, vec3 viewDir);
vec4  directionalLight(Light light, vec4 objColor, vec3 pos, vec3 normal, vec3 viewDir, int colorMode);
vec4  pointLight(Light light, vec4 objColor, vec3 pos, vec3 normal, vec3 viewDir, int colorMode);
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

float computeAttenuation(Light light, float dist)
{
    return 1.f / (light.attParamConst + light.attParamLinear * dist + light.attParamQuad * (dist * dist));
}

vec4 materialColor(Material material, vec2 texCoord)
{
    vec4 color;
    bool hasTexture = material.hasTexture != NO_TEXTURE;

    if(material.texType == TEX_TYPE_DIFFUSE)
    {
        color = hasTexture ? texture(material.diffuseTexture, texCoord) : vec4(material.kd, 1.f);
    }
    else
    {
        // in this case, if material does not have a texture, we also use kd.
        color = hasTexture ? texture(material.specTexture, texCoord) : vec4(material.kd, 1.f);
    }
    return color;
}

vec4 lightedSurface(Light light, Material material, vec3 pos, vec3 normal, vec2 texCoord, vec3 viewDir)
{
    vec4 surface = vec4(0.f);

    vec4 objColor = materialColor(material, texCoord);

    if(light.type == LIGHT_TYPE_DIRECTIONAL)
    {
        surface = directionalLight(light, objColor, pos, normal, viewDir, material.colorMode);
    } 
    else if(light.type == LIGHT_TYPE_POINT)
    {
        surface = pointLight(light, objColor, pos, normal, viewDir, material.colorMode);
    }

    return surface;
}

vec4 directionalLight(Light light, vec4 objColor, vec3 pos, vec3 normal, vec3 viewDir, int colorMode)
{
    vec4 la = vec4(light.ka * light.ia, 1.f); // light's ambient 
    vec4 ld = vec4(light.kd * light.id, 1.f); // light's diffuse

    vec3  lightDir = -normalize(light.direction);
    float diff = max(dot(normal, lightDir), 0.f);

    vec4 ambient = la * objColor;
    vec4 diffuse = ld * diff * objColor;

    vec4 color = diffuse;

    if(colorMode == COLOR_MODE_ALL) {
        vec4 ls = vec4(light.ks * light.is, 1.f); // light's specular
        // blinn-phong mode
        vec3  halfwayDir = normalize(lightDir + viewDir);
        float specFactor = pow(max(dot(halfwayDir, normal), 0.f), light.shininess);
        vec4  specular = ls * specFactor * objColor;

        color += specular;
    }

    float shadow = shadowCalculation(pos, normal, lightDir);

    color = (1.f - shadow) * color;

    color += ambient;

    return color;
}

vec4 pointLight(Light light, vec4 objColor, vec3 pos, vec3 normal, vec3 viewDir, int colorMode)
{
    vec4 la = vec4(light.ka * light.ia, 1.f); // light's ambient 
    vec4 ld = vec4(light.kd * light.id, 1.f); // light's diffuse

    vec3 lightDir = normalize(light.pos - pos);
    float dist = length(light.pos - pos);
    float fade = computeAttenuation(light, dist);

    float diff = max(dot(normal, lightDir), 0.f);
    
    vec4 ambient = la * objColor * fade;
    vec4 diffuse = ld * diff * objColor * fade;

    vec4 color = diffuse;

    if(colorMode == COLOR_MODE_ALL) {
        vec4 ls = vec4(light.ks * light.is, 1.f); // light's specular
        // blinn-phong mode
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float specFactor = pow(max(dot(halfwayDir, normal), 0.f), light.shininess);
        vec4 specular = ls * specFactor * objColor * fade;

        color += specular;
    }

    float shadow = shadowCalculation(pos, normal, lightDir);

    color = (1.f - shadow) * color;

    color += ambient;

    return color;
}

float shadowCalculation(vec3 pos, vec3 normal, vec3 lightDir)
{
    vec4 lightSpacePos = u_lightPrjMat * u_lightViewMat * vec4(pos, 1.f);
    // LightPos ranges from [-1, 1]
    vec3 lightSpaceCoord = lightSpacePos.xyz / lightSpacePos.w;
    // Clamp to [0, 1]
    lightSpaceCoord = lightSpaceCoord * 0.5f + 0.5f;
    float closestDepth = texture(u_depthTexture, lightSpaceCoord.xy).r;
    float curDepth = lightSpaceCoord.z;
    // Shadow bias: fix shadow acne
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  
    return (curDepth  - bias > closestDepth ? 1.f : 0.f);
}
