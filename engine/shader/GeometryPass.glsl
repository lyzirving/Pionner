/*// -- Pionner Engine Geometry Pass Shader --
// -------------------------------------------
// Filling the G-Buffer
*/
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

uniform bool u_useNormalMap;
uniform mat4 u_modelMat;

uniform bool u_playAnimation;
uniform samplerBuffer u_gpuAnimBuffer;
uniform bool u_bGpuAnimated;
uniform int u_frameIndex;
uniform int u_boneNum;

uniform bool u_bOutline;
uniform bool u_bStatic;
uniform float u_outlineScale;

out VsOut {
    vec3 v_worldPos;
    vec3 v_normal;
    vec2 v_texCoord;
    flat mat3 v_TBN;
}; 

void main() {
    mat4 boneTransform = CalcBoneTransform(u_playAnimation, u_bGpuAnimated, a_weights, a_boneIds, u_frameIndex, u_boneNum, u_gpuAnimBuffer);	

	vec4 pos = boneTransform * vec4(a_pos, 1.f);
    vec3 n = u_bStatic ? a_pos : CalcNormal(u_modelMat * boneTransform, a_normal);
    pos += u_bOutline ? vec4(n * u_outlineScale, 0.f) : vec4(0.f);

	v_worldPos = vec3(u_modelMat * pos);
    v_normal = a_normal;
    v_texCoord = a_texcoord;
    v_TBN = u_useNormalMap ? CalcTBN(u_modelMat, boneTransform, a_normal, a_tangent) : CalcTBN(u_modelMat, boneTransform);
    mat4 prjMat = (u_matrices.PrjType == 0) ? u_matrices.PrjMat : u_matrices.Ortho;
	gl_Position = prjMat * u_matrices.ViewMat * u_modelMat * pos;
}

#version 430 core 
#pragma stage : frag 
precision mediump float;

#include <Materials.glslh>
#include <Normal.glslh>

uniform bool u_useNormalMap;
uniform MaterialAttributes u_material;

uniform bool u_bOutline;
uniform vec4 u_outlineColor;

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
    if(u_bOutline) {   
        gPosition = v_worldPos;
        // gNormal.w is type component, 3 for outline
        gNormal = vec4(v_normal, 3.f);
        gAlbedoAlpha = u_outlineColor;
        gMaterial = vec3(0.5f, 0.f, 1.f);
        gEmission = vec3(0.f);
    }
    else {
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
}