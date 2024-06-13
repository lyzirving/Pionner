#version 430 core
#pragma stage : vert
precision mediump float;

#include <Buffers.glslh>
#include <Normal.glslh>
#include <Skeleton.glslh>

layout (location = 0) in vec3  a_pos;
layout (location = 1) in vec3  a_normal;
layout (location = 2) in vec3  a_tangent;
layout (location = 3) in vec3  a_binormal;
layout (location = 4) in vec2  a_texcoord;
layout (location = 5) in vec4  a_weights;
layout (location = 6) in uvec4 a_boneIds; 

uniform bool u_playAnimation;
uniform bool u_useNormalMap;
uniform mat4 u_modelMat;

uniform samplerBuffer u_gpuAnimBuffer;
uniform bool u_bGpuAnimated;
uniform int u_frameIndex;
uniform int u_boneNum;

out VsOut {
    vec3 v_worldPos;
    vec3 v_normal;
    vec2 v_texCoord;
    flat mat3 v_TBN;
}; 

void main() {
	mat4 boneTransform = u_playAnimation ? a_weights[0] * (u_bGpuAnimated ? (gpuSkeleton(u_frameIndex, u_boneNum, int(a_boneIds[0]), u_gpuAnimBuffer)) : (u_bones.Transform[a_boneIds[0]])) : mat4(1.f);
	boneTransform += u_playAnimation ? a_weights[1] * (u_bGpuAnimated ? (gpuSkeleton(u_frameIndex, u_boneNum, int(a_boneIds[1]), u_gpuAnimBuffer)) : (u_bones.Transform[a_boneIds[1]])) : mat4(0.f);
	boneTransform += u_playAnimation ? a_weights[2] * (u_bGpuAnimated ? (gpuSkeleton(u_frameIndex, u_boneNum, int(a_boneIds[2]), u_gpuAnimBuffer)) : (u_bones.Transform[a_boneIds[2]])) : mat4(0.f);
	boneTransform += u_playAnimation ? a_weights[3] * (u_bGpuAnimated ? (gpuSkeleton(u_frameIndex, u_boneNum, int(a_boneIds[3]), u_gpuAnimBuffer)) : (u_bones.Transform[a_boneIds[3]])) : mat4(0.f);

	vec4 pos = boneTransform * vec4(a_pos, 1.f);
	v_worldPos = vec3(u_modelMat * pos);
    v_normal = a_normal;
    v_texCoord = a_texcoord;
    v_TBN = u_useNormalMap ? CalcTBN(u_modelMat, boneTransform, a_normal, a_tangent) : CalcTBN(u_modelMat, boneTransform);
	gl_Position = u_matrices.PrjMat * u_matrices.ViewMat * u_modelMat * pos;
}

#version 430 core 
#pragma stage : frag 
precision mediump float;

#include <Materials.glslh>
#include <Normal.glslh>

uniform bool u_useNormalMap;
uniform MaterialAttributes u_material;

in VsOut {
    vec3 v_worldPos;
    vec3 v_normal;
    vec2 v_texCoord;
    flat mat3 v_TBN;
}; 

// Different render target for G-Buffer
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec4 gNormal;      // normal(3) + type(1)
layout (location = 2) out vec4 gAlbedoAlpha;
layout (location = 3) out vec3 gMaterial;    // roughness + metalness + ao
layout (location = 4) out vec3 gEmission;

void main() {
    vec4 albedoColor = texture(u_material.AlbedoTexture, v_texCoord);

    gPosition = v_worldPos;
    // w is type component, 2 for mesh
    gNormal = vec4(normalize(v_TBN * (u_useNormalMap ? FetchNormal(u_material.NormalTexture, v_texCoord) : v_normal)), 2.f);
    gAlbedoAlpha = vec4(albedoColor.rgb * u_material.AlbedoColor, albedoColor.a);
    // In metallic-roughness flow, g channel is roughness factor, b is metallic factor
    gMaterial.x = texture(u_material.MetallicRoughnessTexture, v_texCoord).g * u_material.Roughness;
    gMaterial.x = max(gMaterial.x, 0.05f);// Minimum roughness of 0.05 to keep specular highlight
    gMaterial.y = texture(u_material.MetallicRoughnessTexture, v_texCoord).b * u_material.Metalness;
    gMaterial.z = texture(u_material.AOTexture, v_texCoord).r * u_material.AO;
    gEmission = texture(u_material.EmissionTexture, v_texCoord).rgb * u_material.Emission;
}