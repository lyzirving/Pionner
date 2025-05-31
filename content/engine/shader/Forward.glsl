/*// -- Pionner Engine Forward Pass Shader --
// -------------------------------------------
// Filling the Forward pass's frame buffer
*/
#version 430 core
#pragma stage : vert
precision mediump float;

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_Texcoord;
layout (location = 2) in vec3 a_Normal;
layout (location = 3) in vec3 a_Tangent;
layout (location = 4) in vec3 a_Bitangent;

#include "Normal.glslh"
#include "Camera.glslh"
#include "Motion.glslh"

out v2f {
    vec3 v_WorldPos;
    vec3 v_Normal;
    vec2 v_TexCoord;
    flat mat3 v_TBN;
    flat mat3 v_InvTBN;
}; 

void main() {     
    v_WorldPos = vec3(u_Motion.Transform * vec4(a_Pos, 1.f));
    v_Normal = normalize(NormalMat(u_Motion.Transform) * a_Normal);
    v_TexCoord = a_Texcoord; 
    v_TBN = mat3(1.f);
    v_InvTBN = mat3(1.f);

    mat4 prjMat = (u_Camera.PrjType == PRJ_TYPE_PERSPECTIVE) ? u_Camera.PrjMat : u_Camera.OrthoMat;
    gl_Position = prjMat * u_Camera.ViewMat * u_Motion.Transform * vec4(a_Pos, 1.f);
}

#version 430 core 
#pragma stage : frag 
precision mediump float;

#include "Camera.glslh"
#include "VisualEffects.glslh"
#include "lighting/DirLitEffect.glslh"
#include "material/PBRMaterial.glslh"

in v2f {
    vec3 v_WorldPos;
    vec3 v_Normal;
    vec2 v_TexCoord;
    flat mat3 v_TBN;
    flat mat3 v_InvTBN;
}; 

vec4 LightContribution();

out vec4 o_FragColor;

void main() {
	vec4 baseColor = texture(u_AlbedoMap, v_TexCoord);	
	vec3 surface = texture(u_MetallicRoughnessMap, v_TexCoord).rgb;    

    m_PBRParams.FragPos = v_WorldPos;
	m_PBRParams.N = v_Normal;
    m_PBRParams.Albedo = baseColor.rgb * u_PBRMaterial.Albedo;
    m_PBRParams.Alpha = baseColor.a;
    // roughness, minimum roughness of 0.05 to keep specular highlight
	m_PBRParams.Roughness = max(surface.g * u_PBRMaterial.Roughness, 0.05);    
    m_PBRParams.Metalness = surface.b * u_PBRMaterial.Metalness;
    m_PBRParams.Emission = texture(u_EmissionMap, v_TexCoord).rgb * u_PBRMaterial.Emission;

    m_PBRParams.V = normalize(u_Camera.Position - m_PBRParams.FragPos);
    m_PBRParams.R = reflect(-m_PBRParams.V, m_PBRParams.N);
    m_PBRParams.NdotV = max(dot(m_PBRParams.N, m_PBRParams.V), 0.f);
    m_PBRParams.F0 = mix(U_F0, m_PBRParams.Albedo, m_PBRParams.Metalness);

    vec4 color = LightContribution();

    //@todo: use macro to do post process
    o_FragColor = vec4(GammaCorrect(color.rgb), color.a);
}

vec4 LightContribution()
{   
    vec3 lightContrib = vec3(0.f);
    lightContrib += LitEffect_DirLit() * LitEffect_DirLitShadow();
    lightContrib += (m_PBRParams.Albedo * m_PBRParams.Emission);
    return vec4(lightContrib.rgb, m_PBRParams.Alpha);
}