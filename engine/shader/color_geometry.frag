// specify the minimum version of opengl glsl, if current version is lower than this, the shader will not complie
#version 330 core

const int LIGHT_TYPE_DIRECTIONAL = 0;
const int LIGHT_TYPE_POINT       = 1;

precision highp float;

struct Light {
    int   type; // 0 for directional light, 1 for point light
    vec3  pos;  // light's position in world coordinate system
    vec3  direction;
    vec3  ka, kd, ks; // color of ambient, diffuse, specular
    float ia, id, is; // intensity of ambient, diffuse, specular
    float shininess;
    float attParamConst, attParamLinear, attParamQuad; // params of attenuation for point light
};

uniform vec4 u_color;
uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_prjMat;

uniform mat4 u_normalMat;

uniform mat4 u_lightViewMat;
uniform mat4 u_lightPrjMat;

uniform sampler2D u_depthTexture;
uniform int       u_calcShadow;

uniform Light     u_light;

in vec3 v_pos;
in vec3 v_normal;

out vec4 o_color;

vec4  lightedGeometry(Light light, vec3 pos, vec3 normal, vec4 color);
float shadowCalculation(vec3 pos, vec3 normal, vec3 lightDir);
float computeDepth(vec3 pos);

void main() {
    vec3 pos = vec3(u_modelMat * vec4(v_pos, 1.f));
    if(u_calcShadow != 0)
    {
        vec3 normal = normalize(vec3(u_normalMat * vec4(v_normal, 0.f)));
        o_color = lightedGeometry(u_light, pos, normal, u_color);
    }
    else
    {
        o_color = u_color;
    }
    gl_FragDepth = computeDepth(v_pos);
}

float shadowCalculation(vec3 pos, vec3 normal, vec3 lightDir)
{
    vec4 lightSpacePos = u_lightPrjMat * u_lightViewMat * vec4(pos, 1.f);
    // lightPos ranges from [-1, 1]
    vec3 lightSpaceCoord = lightSpacePos.xyz / lightSpacePos.w;
    // clamp to [0, 1]
    lightSpaceCoord = lightSpaceCoord * 0.5f + 0.5f;
    float closestDepth = texture(u_depthTexture, lightSpaceCoord.xy).r;
    float curDepth = lightSpaceCoord.z;
    // Shadow bias: fix shadow acne
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  
    return (curDepth - bias > closestDepth ? 1.f : 0.f);
}

vec4  lightedGeometry(Light light, vec3 pos, vec3 normal, vec4 color)
{
    vec4 la = vec4(light.ka * light.ia, 1.f); // light's ambient 
    vec4 ambient = la * color;

    vec3 lightDir = vec3(0.f, 0.f, 0.f);
    if(light.type == LIGHT_TYPE_DIRECTIONAL)
    {
        lightDir = -normalize(light.direction);
    }
    else if(light.type == LIGHT_TYPE_POINT)
    {
        lightDir = normalize(light.pos - pos);
    }
    float shadow = shadowCalculation(pos, normal, lightDir);

    vec4 result = ambient + (1.f - shadow) * color;
    return result;
}

float computeDepth(vec3 pos)
{
    vec4 posClipSpace = u_prjMat * u_viewMat * u_modelMat * vec4(pos, 1.f);
    return (posClipSpace.z / posClipSpace.w);
}
