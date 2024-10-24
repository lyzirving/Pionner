#version 430 core
#pragma stage : vert
precision mediump float;

#include <Buffers.glslh>
#include <Normal.glslh>
#include <Materials.glslh>

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texcoord;
layout (location = 2) in vec3 a_normal;
layout (location = 3) in vec3 a_tangent;
layout (location = 4) in vec3 a_bitangent;

uniform mat4 u_modelMat;

out v2f {
    vec3 v_worldPos;
    vec3 v_normal;
    vec2 v_texCoord;
    flat mat3 v_TBN;
}; 

void main() {
    mat3 normalMat = NormalMat(u_modelMat);

    v_worldPos = vec3(u_modelMat * vec4(a_pos, 1.f));
    v_texCoord = a_texcoord;
    v_normal = u_pbrMaterial.UseNormalMap ? a_normal : (NormalMat(u_modelMat) * a_normal);
    v_TBN = u_pbrMaterial.UseNormalMap ? TBN(u_modelMat, a_tangent, a_bitangent, a_normal) : mat3(1.f);

    mat4 prjMat = (u_camera.PrjType == 0) ? u_camera.PrjMat : u_camera.Ortho;
    gl_Position = prjMat * u_camera.ViewMat * u_modelMat * vec4(a_pos, 1.f);
}

#version 430 core 
#pragma stage : frag 
precision mediump float;

#include <Normal.glslh>
#include <Materials.glslh>
#include <Lighting.glslh>

in v2f {
    vec3 v_worldPos;
    vec3 v_normal;
    vec2 v_texCoord;
    flat mat3 v_TBN;
}; 

out vec4 o_color;

void main() {
	vec4 baseColor = texture(u_pbrMaterial.AlbedoTexture, v_texCoord);	
	vec4 metallicRoughness = texture(u_pbrMaterial.MetallicRoughness, v_texCoord);

    m_PBRParams.FragPos = v_worldPos;
    m_PBRParams.Albedo = baseColor.rgb * u_pbrMaterial.AlbedoColor;
    m_PBRParams.Alpha = u_pbrMaterial.Alpha;    
    m_PBRParams.N = (u_pbrMaterial.UseNormalMap ? (v_TBN * FetchNormal(u_pbrMaterial.NormalMap, v_texCoord)) : v_normal);
    m_PBRParams.Roughness = max(metallicRoughness.g * u_pbrMaterial.Roughness, 0.05);
    m_PBRParams.Metalness = metallicRoughness.b * u_pbrMaterial.Metalness;
    m_PBRParams.Emission = texture(u_pbrMaterial.EmissionTexture, v_texCoord).rgb * u_pbrMaterial.Emission;

	m_PBRParams.V = normalize(u_camera.Position - m_PBRParams.FragPos);
    m_PBRParams.R = reflect(-m_PBRParams.V, m_PBRParams.N);
    m_PBRParams.NdotV = max(dot(m_PBRParams.N, m_PBRParams.V), 0.f);
    m_PBRParams.F0 = mix(U_F0, m_PBRParams.Albedo, m_PBRParams.Metalness);

    vec3 lightContrib = CalculateDirLightsEffect() + m_PBRParams.Albedo * m_PBRParams.Emission;
    lightContrib = GammaCorrect(lightContrib);
    o_color = vec4(lightContrib, m_PBRParams.Alpha);
}