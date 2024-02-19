#version 400 core
precision highp float;

// NOTE: should not use 1 for type, because component's default value might be 1
const int FRAG_TYPE_MESH    = 2;
const int FRAG_TYPE_OUTLINE = 3;

const int ShadowMode_Hard = 0;
const int ShadowMode_Soft = 1;
const int ShadowMode_Soft_2X = 2;
const int ShadowMode_Soft_4X = 3;

// U_F0 is surface reflection at zero incidence(light directly hits the surface)
// We make the simplifying assumption that most dielectric surfaces 
// look visually correct with a constant U_F0 of 0.04.
const vec3 U_F0 = vec3(0.04f);

const float Epsilon = 0.00001;

const float PI = 3.14159265359;
const float MAX_REFLECTION_LOD = 4.f;

struct PBRParameters
{
    vec3 FragPos;
    vec3 Albedo;
    vec3 Emission;
	float Roughness;
	float Metalness;
    float Alpha;
    float AO;

    vec3 V;
    vec3 N;
    vec3 R;
    vec3 F0;
    float NdotV;
} m_params;

layout (std140) uniform Matrices
{
	mat4 ViewMat;
	mat4 PrjMat;
	mat4 Ortho;
    vec3 CameraPosition;
    float FrustumFar;
} u_matrices;

layout (std140) uniform SdMatrices
{
	mat4 ViewMat;
	mat4 PrjMat;
} u_sdMatrices;

layout (std140) uniform DirectionalLight
{
	vec3 Direction;
    vec3 Radiance;
    float Intensity;
    float Bias;
    int SdMode;
    bool CastShadow;
} u_distantLight;

uniform sampler2D u_GPosition;    // vec3
uniform sampler2D u_GNormal;      // vec4 noraml(3) + type(1)
uniform sampler2D u_GAlbedoAlpha; // vec4 albedo(3) + alpha(1)
uniform sampler2D u_GMaterial;    // vec3 roughness + metalness + ao
uniform sampler2D u_GEmission;    // vec3

uniform sampler2D u_shadowMap;

uniform vec4 u_bgColor;

in vec2 v_texcoord; 
out vec4 o_color;

int nearestInt(float val);
vec4 meshColor();
vec3 calculateDistantLights();
float calcShadow(int mode);

float distantLightHardShadow();
float distantLightPCFShadow(int radius);

float NdfGGX(float NdotH, float roughness);
vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);
float GaSchlickGGX(float NdotL, float NdotV, float roughness);
float GaSchlickG1(float cosTheta, float k);

void main() {
    vec4 baseColor = texture(u_GAlbedoAlpha, v_texcoord);
    vec4 nt = texture(u_GNormal, v_texcoord);// normal and type
    vec3 surface = texture(u_GMaterial, v_texcoord).rgb;

    m_params.FragPos = texture(u_GPosition, v_texcoord).rgb;
    m_params.Albedo = baseColor.rgb;
    m_params.Alpha = baseColor.a;
    m_params.N = nt.xyz;
    m_params.Roughness = surface.r;
    m_params.Metalness = surface.g;
    m_params.AO = surface.b;
    m_params.Emission = texture(u_GEmission, v_texcoord).rgb;

	int type = nearestInt(nt.w);
    o_color = (type == FRAG_TYPE_MESH) ? meshColor() : ((type == FRAG_TYPE_OUTLINE) ? baseColor : u_bgColor);
}

int nearestInt(float val)
{
    float index = floor(val + 0.5f);
	return int(index);
}

vec4 meshColor()
{
    // TODO: use an unified ambient
    vec3 ambient = vec3(0.03f) * m_params.Albedo * m_params.AO;
    vec3 lightEffect = calculateDistantLights(); 
    vec3 color = u_distantLight.CastShadow ? ((1.f - calcShadow(u_distantLight.SdMode)) * lightEffect) : lightEffect;
    
    // TODO: IBL
    color += ambient + m_params.Emission;
    return vec4(color.rgb, m_params.Alpha);
}

vec3 calculateDistantLights()
{
    m_params.V = normalize(u_matrices.CameraPosition - m_params.FragPos);
    m_params.R = reflect(-m_params.V, m_params.N);
    m_params.NdotV = max(dot(m_params.N, m_params.V), 0.f);
    m_params.F0 = mix(U_F0, m_params.Albedo, m_params.Metalness);
    // Negate the direction to expect a direction from the fragment towards the light source
    vec3 Li = normalize(-u_distantLight.Direction);
    vec3 Lh = normalize(Li + m_params.V);
	vec3 Lradiance = u_distantLight.Radiance * u_distantLight.Intensity;

    // Calculate angles between surface normal and light vector.
	float NdotL = max(0.f, dot(m_params.N, Li));
	float NdotH = max(0.f, dot(m_params.N, Lh));

    vec3 F = FresnelSchlickRoughness(max(0.f, dot(Lh, m_params.V)), m_params.F0, m_params.Roughness);
    float D = NdfGGX(NdotH, m_params.Roughness);
    float G = GaSchlickGGX(NdotL, m_params.NdotV, m_params.Roughness);

    vec3 kd = (1.0 - F) * (1.0 - m_params.Metalness);
	vec3 diffuseBRDF = kd * m_params.Albedo;

    // Cook-Torrance
	vec3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * NdotL * m_params.NdotV);
	specularBRDF = clamp(specularBRDF, vec3(0.0f), vec3(10.0f));
    return (diffuseBRDF + specularBRDF) * NdotL * Lradiance;
}

float calcShadow(int mode)
{
    float shadow = 0.f;
    if(mode == ShadowMode_Hard)
    {
        shadow = distantLightHardShadow();
    }
    else if(mode == ShadowMode_Soft_2X)
    {
        shadow = distantLightPCFShadow(2);
    }
    else if(mode == ShadowMode_Soft_4X)
    {
        shadow = distantLightPCFShadow(4);
    }
    else
    {
        shadow = distantLightPCFShadow(1);
    }
    return shadow;
}

float distantLightHardShadow()
{
    vec4 lightSpacePos = u_sdMatrices.PrjMat * u_sdMatrices.ViewMat * vec4(m_params.FragPos, 1.f);
    // perform perspective divide
    vec3 prjCoords = lightSpacePos.xyz / lightSpacePos.w;
    // transform to [0,1] range
    prjCoords = prjCoords * 0.5 + 0.5;
    // get depth of current fragment from light's perspective
    float currentDepth = prjCoords.z;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_shadowMap, prjCoords.xy).r; 

    vec3 lightDir = normalize(-u_distantLight.Direction);   
    float bias = max(u_distantLight.Bias * (1.0 - dot(m_params.N, lightDir)), u_distantLight.Bias); 

    return (currentDepth - bias) > closestDepth  ? 1.0 : 0.0;
}

float distantLightPCFShadow(int radius)
{   
    vec4 lightSpacePos = u_sdMatrices.PrjMat * u_sdMatrices.ViewMat * vec4(m_params.FragPos, 1.f);
    // perform perspective divide
    vec3 prjCoords = lightSpacePos.xyz / lightSpacePos.w;
    // transform to [0,1] range
    prjCoords = prjCoords * 0.5 + 0.5;
    // get depth of current fragment from light's perspective
    float currentDepth = prjCoords.z;

    float shadow = 0.f;
    vec3 lightDir = normalize(-u_distantLight.Direction);    
    float bias = max(u_distantLight.Bias * (1.0 - dot(m_params.N, lightDir)), u_distantLight.Bias); 
    vec2 texelSize = 1.0 / textureSize(u_shadowMap, 0);
    int itrCnt = 0;

    //PCF
    for(int x = -radius; x <= radius; ++x)
    {
        for(int y = -radius; y <= radius; ++y)
        {
            float pcfDepth = texture(u_shadowMap, prjCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += (currentDepth - bias > pcfDepth) ? 1.f : 0.f;  
            itrCnt++;
        }    
    }
    shadow /= float(itrCnt);

    // keep the shadow at 0.0 when outside the far plane region of the light's frustum.
    if(prjCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
} 

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2
float NdfGGX(float NdotH, float roughness)
{
	float alpha = roughness * roughness;
	float alphaSq = alpha * alpha;

	float denom = (NdotH * NdotH) * (alphaSq - 1.0) + 1.0;
	return alphaSq / (PI * denom * denom);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float GaSchlickGGX(float NdotL, float NdotV, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
	return GaSchlickG1(NdotL, k) * GaSchlickG1(NdotV, k);
}

// Single term for separable Schlick-GGX below.
float GaSchlickG1(float cosTheta, float k)
{
	return cosTheta / (cosTheta * (1.0 - k) + k);
}