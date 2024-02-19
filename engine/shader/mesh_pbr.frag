#version 330 core
precision mediump float;

// U_F0 is surface reflection at zero incidence(light directly hits the surface)
// We make the simplifying assumption that most dielectric surfaces 
// look visually correct with a constant U_F0 of 0.04.
const vec3 U_F0 = vec3(0.04f);

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
};

struct MaterialAttributes 
{
    vec3 AlbedoColor;
    float Emission;
    float Roughness;
    float Metalness;
    float AO;
    bool UseNormalMap;

    sampler2D AlbedoTexture;
    sampler2D EmissionTexture;
    sampler2D RoughnessTexture;
    sampler2D MetalnessTexture;
    sampler2D NormalTexture;
    sampler2D AOTexture;
};

struct PBRParameters
{
    vec3  Albedo;
    vec3  Emission;
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

in VsOut {
    vec3 v_worldPos;
    vec3 v_normal;
    vec2 v_texCoord;
    flat mat3 v_TBN;
}; 

uniform bool u_useNormalMap;
uniform mat4 u_modelMat;
uniform MaterialAttributes u_material;
uniform samplerCube u_pointLightDepthBuffer;

vec3 calcNormal();
vec3 calcPointLightsOut(PointLight pointLight);
float calcDepth();

float normalDistributionGGX(vec3 N, vec3 H, float roughness);

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
float geometrySchlickGGX(float NdotV, float roughness);

vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);

float softShadow(PointLight pointLight);
float pointLightSoftShadow(PointLight pointLight);

out vec4 o_color;

void main() {
    vec4 albedoColor = texture(u_material.AlbedoTexture, v_texCoord);
    m_params.Albedo = albedoColor.rgb * u_material.AlbedoColor;
    m_params.Alpha = albedoColor.a;
    m_params.Roughness = texture(u_material.RoughnessTexture, v_texCoord).r * u_material.Roughness;
    m_params.Metalness = texture(u_material.MetalnessTexture, v_texCoord).r * u_material.Metalness;
    m_params.Emission = texture(u_material.EmissionTexture, v_texCoord).rgb * u_material.Emission;
    m_params.AO = texture(u_material.AOTexture, v_texCoord).r * u_material.AO;
    // Minimum roughness of 0.05 to keep specular highlight
    m_params.Roughness = max(m_params.Roughness, 0.05f);

    m_params.V = normalize(u_matrices.CameraPosition - v_worldPos);
    m_params.N = calcNormal();
    m_params.R = reflect(-m_params.V, m_params.N);
    m_params.NdotV = max(dot(m_params.N, m_params.V), 0.f);
    m_params.F0 = mix(U_F0, m_params.Albedo, m_params.Metalness);

    vec3 Lo = calcPointLightsOut(u_pointLightData.Lights[0]);
    Lo = (1.f - softShadow(u_pointLightData.Lights[0])) * Lo;

    // TODO: IBL
    vec3 ambient = vec3(0.03) * m_params.Albedo * m_params.AO;

    // emission is not affected by lighting
    vec3 color = ambient + m_params.Emission + Lo;
    o_color = vec4(color.rgb, m_params.Alpha);
}

vec3 calcNormal()
{
    vec3 N;
    if(u_useNormalMap)
    {
        N = texture(u_material.NormalTexture, v_texCoord).rgb;
        // turn normal from [0, 1] into [-1, 1]
        // now normal is in tangent space
        N = 2.f * N - 1.f;
    }
    else
    {
        N = v_normal;
    }
    return normalize(v_TBN * N);
}

vec3 calcPointLightsOut(PointLight pointLight)
{
    // TODO: implement multiple point lights
    vec3 L = normalize(pointLight.Position - v_worldPos);
    vec3 H = normalize(m_params.V + L);
    float NdotL = max(dot(m_params.N, L), 0.f); 
    float lightDistance = length(pointLight.Position - v_worldPos);
    float attenuation = clamp(1.0 - (lightDistance * lightDistance) / (pointLight.Radius * pointLight.Radius), 0.f, 1.f);
    attenuation *= mix(attenuation, 1.0, pointLight.Falloff);
    // float attenuation = 1.f / (lightDistance * lightDistance);

    // light radiance
    vec3 radiance = pointLight.Radiance * pointLight.Intensity * attenuation;

    // Cook-Torrance BRDF
    float NDF = normalDistributionGGX(m_params.N, H, m_params.Roughness);  
    float G = geometrySmith(m_params.N, m_params.V, L, m_params.Roughness);  
    vec3 F = fresnelSchlick(clamp(dot(H, m_params.V), 0.0, 1.0), m_params.F0);

    vec3 numerator = NDF * G * F; 
    // + 0.0001 to prevent divide by zero
    float denominator = 4.0 * m_params.NdotV * NdotL + 0.0001;
    // Cook-Torrance specular
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    // For energy conservation, the diffuse and specular light can't
    // be above 1.0 
    vec3 kD = vec3(1.0) - kS;
    kD *= (1.0 - m_params.Metalness);

    vec3 Lo = (kD * m_params.Albedo / PI + specular) * radiance * NdotL;
    return Lo;
}

float normalDistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float  NdotV = max(dot(N, V), 0.0);
    float  NdotL = max(dot(N, L), 0.0);
    float  ggx2 = geometrySchlickGGX(NdotV, roughness);
    float  ggx1 = geometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

float geometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
} 

float softShadow(PointLight pointLight)
{
    return pointLightSoftShadow(pointLight);
}

float pointLightSoftShadow(PointLight pointLight)
{
    vec3 fragToLight = v_worldPos - pointLight.Position;
    vec3 lightDir = normalize(-fragToLight);
    float currentDepth = length(fragToLight);

    float shadow = 0.f;
    float bias = max(0.005f * (1.0 - dot(m_params.N, lightDir)), (0.005f)); 
    int sampleCnt = 20;

    float viewDist = length(u_matrices.CameraPosition - v_worldPos);
    float diskRadius = (1.f + (viewDist / u_matrices.FrustumFar)) / 25.f;

    for(int i = 0; i < sampleCnt; ++i)
    {
        float closestDepth = texture(u_pointLightDepthBuffer, fragToLight + normalize(SPL_DIR[i]) * diskRadius).r;
        closestDepth *= u_matrices.FrustumFar;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(sampleCnt);
    return shadow;
}

float calcDepth()
{
    vec4 posClipSpace = u_matrices.PrjMat * u_matrices.ViewMat * vec4(v_worldPos, 1.f);
    return (posClipSpace.z / posClipSpace.w);
}