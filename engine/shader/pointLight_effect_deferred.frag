#version 400 core
precision highp float;

// NOTE: should not use 1 for type, because component's default value might be 1
const int FRAG_TYPE_MESH    = 2;
const int FRAG_TYPE_OUTLINE = 3;

// U_F0 is surface reflection at zero incidence(light directly hits the surface)
// We make the simplifying assumption that most dielectric surfaces 
// look visually correct with a constant U_F0 of 0.04.
const vec3 U_F0 = vec3(0.04f);

const float Epsilon = 0.00001;

const int POINT_LIGHT_LIMIT = 10;

const vec3 SPL_DIR[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

const float PI = 3.14159265359;
const float MAX_REFLECTION_LOD = 4.f;

struct PointLight
{
    vec3 Position;
    vec3 Radiance;
    float Intensity;
	float MinRadius;
	float Radius;
	float Falloff;
	float SourceSize;
    bool CastShadow;
};

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

layout(std140) uniform PointLightData
{
	int LightCount;
	PointLight Lights[POINT_LIGHT_LIMIT];
} u_pointLightData;

uniform sampler2D u_GPosition;   // vec3
uniform sampler2D u_GNormal;     // vec4 noraml(3) + type(1)
uniform sampler2D u_GAlbedoAlpha;// vec4 albedo(3) + alpha(1)
uniform sampler2D u_GMaterial;   // vec3 roughness + metalness + ao
uniform sampler2D u_GEmission;   // vec3

uniform samplerCubeArray u_pointLightDepthBuffer;
uniform vec4 u_bgColor;

in vec2 v_texcoord; 
out vec4 o_color;

vec3 calcPointLightOut(PointLight pointLight);

vec3  FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);
float NdfGGX(float NdotH, float roughness);
float GaSchlickGGX(float NdotL, float NdotV, float roughness);
float GaSchlickG1(float cosTheta, float k);

float softShadow(int lightIndex, PointLight pointLight);
float pointLightSoftShadow(int lightIndex, PointLight pointLight);

vec3 mon2lin(vec3 x);
int  nearestInt(float val);
vec4 meshColor();

void main() {
    vec4 baseColor = texture(u_GAlbedoAlpha, v_texcoord);
    vec4 nt = texture(u_GNormal, v_texcoord);// normal and type
    vec3 surface = texture(u_GMaterial, v_texcoord).rgb;

    m_params.FragPos = texture(u_GPosition, v_texcoord).rgb;
    m_params.Albedo = mon2lin(baseColor.rgb);
    m_params.Alpha = baseColor.a;
    m_params.N = nt.xyz;
    m_params.Roughness = surface.r;
    m_params.Metalness = surface.g;
    m_params.AO = surface.b;
    m_params.Emission = texture(u_GEmission, v_texcoord).rgb;

	int type = nearestInt(nt.w);
    o_color = (type == FRAG_TYPE_MESH) ? meshColor() : vec4(0.f);
}

vec3 calcPointLightOut(PointLight pointLight)
{
    vec3 L = normalize(pointLight.Position - m_params.FragPos);
    vec3 H = normalize(m_params.V + L);
    float NdotL = max(dot(m_params.N, L), 0.f); 
    float NdotH = max(dot(m_params.N, H), 0.f); 
    float lightDistance = length(pointLight.Position - m_params.FragPos);
    float attenuation = clamp(1.0 - (lightDistance * lightDistance) / (pointLight.Radius * pointLight.Radius), 0.f, 1.f);
    attenuation *= mix(attenuation, 1.0, pointLight.Falloff);
    // float attenuation = 1.f / (lightDistance * lightDistance);

    // light radiance
    vec3 radiance = pointLight.Radiance * pointLight.Intensity * attenuation;

    // Cook-Torrance BRDF
    vec3 F = FresnelSchlickRoughness(max(0.f, dot(H, m_params.V)), m_params.F0, m_params.Roughness);
    float NDF = NdfGGX(NdotH, m_params.Roughness);  
    float G = GaSchlickGGX(NdotL, m_params.NdotV, m_params.Roughness);

    // Cook-Torrance specular
    vec3 specular = (NDF * G * F) / max(Epsilon, 4.0 * NdotL * m_params.NdotV);
    specular = clamp(specular, vec3(0.0f), vec3(10.0f));

    vec3 ks = F;
    vec3 kd = (vec3(1.0) - ks) * (1.0 - m_params.Metalness);// For energy conservation
    vec3 diffuse = kd * m_params.Albedo;

    vec3 Lo = (diffuse + specular) * radiance * NdotL;
    return Lo;
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

float softShadow(int lightIndex, PointLight pointLight)
{
    return pointLightSoftShadow(lightIndex, pointLight);
}

float pointLightSoftShadow(int lightIndex, PointLight pointLight)
{
    vec3 fragToLight = m_params.FragPos - pointLight.Position;
    vec3 lightDir = normalize(-fragToLight);
    float currentDepth = length(fragToLight);

    float shadow = 0.f;
    float bias = max(0.005f * (1.0 - dot(m_params.N, lightDir)), (0.005f)); 
    int sampleCnt = 20;

    float viewDist = length(u_matrices.CameraPosition - m_params.FragPos);
    float diskRadius = (1.f + (viewDist / u_matrices.FrustumFar)) / 25.f;

    for(int i = 0; i < sampleCnt; ++i)
    {
        vec4 sampleCoord = vec4(fragToLight + normalize(SPL_DIR[i]) * diskRadius, float(lightIndex));
        float closestDepth = texture(u_pointLightDepthBuffer, sampleCoord).r;
        closestDepth *= u_matrices.FrustumFar;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(sampleCnt);
    return shadow;
}

vec4 meshColor()
{
    m_params.V = normalize(u_matrices.CameraPosition - m_params.FragPos);
    m_params.R = reflect(-m_params.V, m_params.N);
    m_params.NdotV = max(dot(m_params.N, m_params.V), 0.f);
    m_params.F0 = mix(U_F0, m_params.Albedo, m_params.Metalness);

    vec3 Lo = vec3(0.f);
    for(int i = 0; i < u_pointLightData.LightCount; i++)
    {
        vec3 cur = calcPointLightOut(u_pointLightData.Lights[i]);
        vec3 affect = u_pointLightData.Lights[i].CastShadow ? (1.f - softShadow(i, u_pointLightData.Lights[i])) * cur : cur;

        Lo += affect;
    }

    // TODO: IBL
    //vec3 ambient = vec3(0.03) * m_params.Albedo * m_params.AO;
    //vec3 color = ambient + m_params.Emission + Lo;

    return vec4(Lo.rgb, m_params.Alpha);
}

vec3 mon2lin(vec3 x)
{
    return vec3(pow(x[0], 2.2), pow(x[1], 2.2), pow(x[2], 2.2));
}

int nearestInt(float val)
{
    float index = floor(val + 0.5f);
	return int(index);
}
