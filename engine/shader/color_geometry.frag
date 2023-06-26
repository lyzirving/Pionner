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

uniform mat3 u_normalMat;

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
        vec3 normal = normalize(u_normalMat * v_normal);
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
    if(lightSpaceCoord.z > 1.f)
    {
        return 0.f;
    }
    // Shadow bias: fix shadow acne
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005); 
    float curDepth = lightSpaceCoord.z;
    //float shadow = curDepth - bias > texture(u_depthTexture, lightSpaceCoord.xy).r  ? 1.0 : 0.0;

    vec2 texelSize = 1.0 / textureSize(u_depthTexture, 0);

    float shadow = 0.f;
    // Pcf: make soft shadow.
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(u_depthTexture, lightSpaceCoord.xy + vec2(x, y) * texelSize).r; 
            //shadow += curDepth - bias > pcfDepth  ? 1.0 : 0.0;   
            // TODO: why if we ignore bias, the shadow will be correct on plane?
            shadow += curDepth > pcfDepth  ? 1.0 : 0.0;     
        }    
    } 
    shadow /= 9.f;
    return shadow;
}

vec4  lightedGeometry(Light light, vec3 pos, vec3 normal, vec4 color)
{
    vec3 la = light.ka * light.ia;// light's ambient 

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

    vec3 colorRgb = la + (1.f - shadow) * color.rgb;

    vec4 result = vec4(colorRgb.rgb, color.a);

    return result;
}

float computeDepth(vec3 pos)
{
    vec4 posClipSpace = u_prjMat * u_viewMat * u_modelMat * vec4(pos, 1.f);
    return (posClipSpace.z / posClipSpace.w);
}
