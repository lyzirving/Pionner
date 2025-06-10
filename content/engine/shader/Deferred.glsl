/*// -- Pionner Engine Deferred Pass Shader --
// -------------------------------------------
// Filling the Deferred pass's frame buffer
*/
#version 430 core
#pragma stage : vert
precision highp float;

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_Texcoord;

out v2f {
    vec2 v_TexCoord;
}; 

void main() {
	v_TexCoord = a_Texcoord;
	gl_Position = vec4(a_Pos, 1.f);
}

#version 430 core 
#pragma stage : frag 
precision highp float;

uniform sampler2D u_GPosition;   // vec3
uniform sampler2D u_GNormal;     // vec4 noraml(3) + type(1)
uniform sampler2D u_GAlbedoAlpha;// vec4 albedo(3) + alpha(1)
uniform sampler2D u_GMaterial;   // vec3 ao(r) + roughness(g) + metalness(b)
uniform sampler2D u_GEmission;   // vec3

in v2f {
    vec2 v_TexCoord;
}; 

#include "SurfaceShading.glslh"
#include "lighting/PointLitEffect.glslh"

void MakeMaterial(inout MaterialInputs material);
void PrepareShading();
vec4 LightContribution();

out vec4 o_FragColor;

void main() {
	vec4 baseColor = texture(u_GAlbedoAlpha, v_TexCoord);	
	vec3 surface = texture(u_GMaterial, v_TexCoord).rgb;

	m_PBRParams.FragPos = texture(u_GPosition, v_TexCoord).xyz;
	m_PBRParams.N = texture(u_GNormal, v_TexCoord).xyz;
    m_PBRParams.Albedo = baseColor.rgb;
    m_PBRParams.Alpha = baseColor.a;
	m_PBRParams.Roughness = surface.g;
    m_PBRParams.Metalness = surface.b;
    m_PBRParams.Occlusion = 1.0;
    m_PBRParams.Emission = texture(u_GEmission, v_TexCoord).rgb;

	m_PBRParams.V = normalize(u_Camera.Position - m_PBRParams.FragPos);
    m_PBRParams.R = reflect(-m_PBRParams.V, m_PBRParams.N);
    m_PBRParams.NdotV = max(dot(m_PBRParams.N, m_PBRParams.V), 0.f);
    m_PBRParams.F0 = mix(U_F0, m_PBRParams.Albedo, m_PBRParams.Metalness);    

    o_FragColor = LightContribution();
}

void MakeMaterial(inout MaterialInputs material) {		
	vec3 surface = texture(u_GMaterial, v_TexCoord).rgb;
	material.baseColor = texture(u_GAlbedoAlpha, v_TexCoord);
	material.roughness = surface.g;
    material.metallic = surface.b;
    material.emissive = vec4(texture(u_GEmission, v_TexCoord).rgb, 1.0);
}

void PrepareShading() {
    m_ShadingPosition = texture(u_GPosition, v_TexCoord).xyz;
	m_ShadingNormal = texture(u_GNormal, v_TexCoord).xyz;
    m_ShadingView = normalize(u_Camera.Position - m_ShadingPosition);
    m_ShadingReflected = reflect(-m_ShadingView, m_ShadingNormal);
    m_ShadingNoV = max(dot(m_ShadingNormal, m_ShadingView), MIN_N_DOT_V);
}

vec4 LightContribution()
{   
    MaterialInputs material;
    InitMaterial(material);
    MakeMaterial(material);
    PrepareShading();
    vec4 color = EvaluateMaterial(material);

    vec3 lightContrib = vec3(0.f);
    //lightContrib += LitEffect_DirLit() * LitEffect_DirLitShadow();
    lightContrib += color.rgb * LitEffect_DirLitShadow();
    lightContrib += LitEffect_PointLits() * LitEffect_PointLitShadow();   
    return vec4(lightContrib.rgb, material.baseColor.a);
}