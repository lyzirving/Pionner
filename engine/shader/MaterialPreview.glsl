/*// -- Pionner Engine Material Preview Shader --
//// --------------------------------------------
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

out vec2 v_texCoord;

void main() {
    mat4 boneTransform = CalcBoneTransform(u_playAnimation, u_bGpuAnimated, a_weights, a_boneIds, u_frameIndex, u_boneNum, u_gpuAnimBuffer);	

	vec4 pos = boneTransform * vec4(a_pos, 1.f);
    vec3 n = u_bStatic ? a_pos : CalcNormal(u_modelMat * boneTransform, a_normal);
    pos += u_bOutline ? vec4(n * u_outlineScale, 0.f) : vec4(0.f);

    v_texCoord = a_texcoord;    
	gl_Position = u_matrices.PrjMat * u_matrices.ViewMat * u_modelMat * pos;
}

#version 430 core 
#pragma stage : frag 
precision mediump float;

#include <Materials.glslh>

uniform MaterialAttributes u_material;

uniform bool u_bOutline;
uniform vec4 u_outlineColor;

in vec2 v_texCoord;

out vec4 o_color;

void main() {
    vec4 albedoColor = texture(u_material.AlbedoTexture, v_texCoord);
    o_color = u_bOutline ? u_outlineColor : vec4(albedoColor.rgb * u_material.AlbedoColor.rgb, albedoColor.a * u_material.Alpha);   
}