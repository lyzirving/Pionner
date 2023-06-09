// specify the minimum version of opengl glsl, if current version is lower than this, the shader will not complie
#version 330 core

precision highp float;

const int LIGHT_TYPE_DIRECTIONAL = 0;
const int LIGHT_TYPE_POINT       = 1;

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

uniform mat4 u_viewMat;
uniform mat4 u_prjMat;

uniform mat4 u_lightViewMat;
uniform mat4 u_lightPrjMat;

uniform sampler2D   u_depthTexture;
uniform samplerCube u_cubeDepthTexture;
uniform int         u_calcShadow;

uniform Light u_light;
uniform vec4  u_color;

uniform vec3  u_lightPos;
uniform vec3  u_viewPos;
uniform vec3  u_lightDir;

uniform float u_farPlane;

in vec3 v_fragPos;
in vec3 v_normal;

out vec4 o_color;

vec4  lightedGeometry(vec3 fragPos, vec3 normal, vec4 color);

float directionLightShadowSoft(vec3 fragPos, vec3 normal, vec3 lightDir);
float pointLightShadow(vec3 fragPos, vec3 normal, vec3 lightDir);
float pointLightShadowSoft(vec3 fragPos, vec3 normal, vec3 lightDir);

float computeDepth(vec3 fragPos);
float computeFade(float dist);

void main() {
    if(u_calcShadow != 0)
    {
        o_color = lightedGeometry(v_fragPos, v_normal, u_color);
    }
    else
    {
        o_color = u_color;
    }
    gl_FragDepth = computeDepth(v_fragPos);
}

float directionLightShadowSoft(vec3 fragPos, vec3 normal, vec3 lightDir)
{
    vec4 lightSpacePos = u_lightPrjMat * u_lightViewMat * vec4(fragPos, 1.f);
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

float pointLightShadow(vec3 fragPos, vec3 normal, vec3 lightDir)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - u_lightPos;
    // ise the fragment to light vector to sample from the depth map    
    float closestDepth = texture(u_cubeDepthTexture, fragToLight).r;
    // it is currently in linear range between [0,1], let's re-transform it back to original depth value
    closestDepth *= u_farPlane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // test for shadows
    // should we need a larger bias?
    float bias = max(0.005f * (1.0 - dot(normal, lightDir)), (0.005f)); 
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;        
         
    return shadow;
}

float pointLightShadowSoft(vec3 fragPos, vec3 normal, vec3 lightDir)
{
    vec3 fragToLight = fragPos - u_lightPos;       
    float currentDepth = length(fragToLight);

    float shadow = 0.f;
    float bias = max(0.005f * (1.0 - dot(normal, lightDir)), (0.005f)); 
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

vec4  lightedGeometry(vec3 fragPos, vec3 normal, vec4 color)
{
    vec3 la = u_light.ka * u_light.ia;// light's ambient 

    vec3 lightDir;
    float shadow;
    
    vec3 colorRgb;
    if(u_light.type == LIGHT_TYPE_POINT)
    {
        lightDir = normalize(u_lightPos - fragPos);
        shadow = pointLightShadowSoft(fragPos, normal, lightDir);

        float dist = length(u_lightPos - fragPos);

        colorRgb = (la + 1.f - shadow) * color.rgb * computeFade(dist);
    }
    else
    {
        lightDir = -normalize(u_lightDir);
        shadow = directionLightShadowSoft(fragPos, normal, lightDir);

        colorRgb = (la + 1.f - shadow) * color.rgb;
    }

    return vec4(colorRgb.rgb, color.a);
}

float computeDepth(vec3 fragPos)
{
    vec4 posClipSpace = u_prjMat * u_viewMat * vec4(fragPos, 1.f);
    return (posClipSpace.z / posClipSpace.w);
}

// attenuation for light's intensity/irradiance(power per area)
float computeFade(float dist)
{
    return 1.f / (u_light.attParamConst + u_light.attParamLinear * dist + u_light.attParamQuad * (dist * dist));
}
