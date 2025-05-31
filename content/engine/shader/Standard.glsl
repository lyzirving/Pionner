/*// ---------- Pionner Engine Standard Material Shader -------------
// ------------------------------------------------------------------
// Filling the G-Buffer cause Engine use defferred shading by default
*/
#version 430 core
#pragma stage : vert
precision mediump float;

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_Texcoord;
layout (location = 2) in vec3 a_Normal;
layout (location = 3) in vec3 a_Tangent;
layout (location = 4) in vec3 a_Bitangent;

#ifdef USE_SKELETON
layout (location = 5) in vec4 a_Weights;
layout (location = 6) in ivec4 a_BoneIds;
#endif

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
#ifdef USE_SKELETON
    mat4 boneMat = Skinning(a_BoneIds, a_Weights);
    v_WorldPos = vec3(u_Motion.Transform * boneMat * vec4(a_Pos, 1.f));
    v_Normal = normalize(NormalMat(u_Motion.Transform * boneMat) * a_Normal);
#else
    v_WorldPos = vec3(u_Motion.Transform * u_Motion.LocalTransform * vec4(a_Pos, 1.f));
    v_Normal = normalize(NormalMat(u_Motion.Transform) * a_Normal);
#endif    
    v_TexCoord = a_Texcoord; 
    v_TBN = mat3(1.f);
    v_InvTBN = mat3(1.f);
    mat4 prjMat = (u_Camera.PrjType == PRJ_TYPE_PERSPECTIVE) ? u_Camera.PrjMat : u_Camera.OrthoMat;
    gl_Position = prjMat * u_Camera.ViewMat * vec4(v_WorldPos, 1.f);
}

#version 430 core 
#pragma stage : frag 
precision mediump float;

#include "material/PBRMaterial.glslh"

in v2f {
    vec3 v_WorldPos;
    vec3 v_Normal;
    vec2 v_TexCoord;
    flat mat3 v_TBN;
    flat mat3 v_InvTBN;
}; 

// Different render target for G-Buffer
layout (location = 0) out vec4 GPosition;
layout (location = 1) out vec4 GNormal;
layout (location = 2) out vec4 GAlbedoAlpha;
layout (location = 3) out vec4 GMaterial;// ao(r) + roughness(g) + metalness(b)
layout (location = 4) out vec4 GEmission;

void main() {
    vec4 albedoColor = texture(u_AlbedoMap, v_TexCoord);
    // In metallic-roughness source, r channel is ao, g channel is roughness, and b is metallic
    vec3 material = texture(u_MetallicRoughnessMap, v_TexCoord).rgb; 

    GPosition = vec4(v_WorldPos, 1.f);      
    GNormal = vec4(v_Normal, 0.f);
    GAlbedoAlpha = vec4(albedoColor.rgb * u_PBRMaterial.Albedo, u_PBRMaterial.Alpha);

    // ao
    GMaterial.r = material.r * u_PBRMaterial.Occlusion;
    // roughness, minimum roughness of 0.05 to keep specular highlight
    GMaterial.g = max(material.g * u_PBRMaterial.Roughness, 0.05);
    // metallic
    GMaterial.b = material.b * u_PBRMaterial.Metalness;
    // padding
    GMaterial.a = 1.f;

    GEmission = vec4(texture(u_EmissionMap, v_TexCoord).rgb * u_PBRMaterial.Emission, 1.f);
}