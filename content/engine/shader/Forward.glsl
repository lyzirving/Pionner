/*// -- Pionner Engine Forward Pass Shader --
// -------------------------------------------
// Filling the Forward pass's frame buffer
*/
#version 430 core
#pragma stage : vert
precision highp float;

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
    v_WorldPos = vec3(u_Motion.Transform * u_Motion.LocalTransform * vec4(a_Pos, 1.f));
    v_Normal = normalize(NormalMat(u_Motion.Transform * u_Motion.LocalTransform) * a_Normal);
    v_TexCoord = a_Texcoord; 
    v_TBN = mat3(1.f);
    v_InvTBN = mat3(1.f);

    mat4 prjMat = (u_Camera.PrjType == PRJ_TYPE_PERSPECTIVE) ? u_Camera.PrjMat : u_Camera.OrthoMat;
    gl_Position = prjMat * u_Camera.ViewMat * vec4(v_WorldPos, 1.f);
}

#version 430 core 
#pragma stage : frag 
precision highp float;

in v2f {
    vec3 v_WorldPos;
    vec3 v_Normal;
    vec2 v_TexCoord;
    flat mat3 v_TBN;
    flat mat3 v_InvTBN;
}; 

#include "CommonGraphics.glslh"
#include "SurfaceShading.glslh"
#include "material/PBRMaterial.glslh"

void MakeMaterial(inout MaterialInputs material);
void PrepareShading();

out vec4 o_FragColor;

void main() {
	MaterialInputs material;
    InitMaterial(material);
    MakeMaterial(material);
    PrepareShading();
    vec3 color = EvaluateMaterial(material);
    //@todo: use macro to do post process
    o_FragColor = vec4(GammaCorrect(color), material.baseColor.a);
}

void MakeMaterial(inout MaterialInputs material) {
    vec4 baseColor = texture(u_AlbedoMap, v_TexCoord);
	vec3 surface = texture(u_MetallicRoughnessMap, v_TexCoord).rgb; 

	material.baseColor = vec4(baseColor.rgb * u_PBRMaterial.Albedo, u_PBRMaterial.Alpha);
    // roughness, minimum roughness of 0.05 to keep specular highlight
	material.roughness = max(surface.g * u_PBRMaterial.Roughness, 0.05);
    material.metallic = surface.b * u_PBRMaterial.Metalness;
    material.emissive = vec4(texture(u_EmissionMap, v_TexCoord).rgb * u_PBRMaterial.Emission, 1.0);
    material.clearCoat = u_PBRMaterial.ClearCoat;
    material.clearCoatRoughness = u_PBRMaterial.ClearCoatRoughness;
}

void PrepareShading() {
    m_ShadingPosition = v_WorldPos;
	m_ShadingNormal = v_Normal;
    m_ShadingView = normalize(u_Camera.Position - m_ShadingPosition);
    m_ShadingReflected = reflect(-m_ShadingView, m_ShadingNormal);
    m_ShadingNoV = max(dot(m_ShadingNormal, m_ShadingView), MIN_N_DOT_V);
}