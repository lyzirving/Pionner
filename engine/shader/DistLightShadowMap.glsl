/*// -- Pionner Engine Directional Light Shadow Map shader --
// -------------------------------------------
// Filling the shadow map for directional light
*/
#version 430 core
#pragma stage : vert
precision mediump float;

#include <Skeleton.glslh>
#include <Lighting.glslh>

layout (location = 0) in vec3  a_pos;
layout (location = 1) in vec3  a_normal;
layout (location = 2) in vec3  a_tangent;
layout (location = 3) in vec3  a_binormal;
layout (location = 4) in vec2  a_texcoord;
layout (location = 5) in vec4  a_weights;
layout (location = 6) in uvec4 a_boneIds;

uniform bool u_playAnimation;
uniform bool u_bGpuAnimated;
uniform int u_frameIndex;
uniform int u_boneNum;
uniform samplerBuffer u_gpuAnimBuffer;

uniform mat4 u_modelMat;

void main() {
	mat4 boneTransform = CalcBoneTransform(u_playAnimation, u_bGpuAnimated, a_weights, a_boneIds, u_frameIndex, u_boneNum, u_gpuAnimBuffer);
	vec4 pos = boneTransform * vec4(a_pos, 1.f);

	gl_Position = u_sdMatrices.PrjMat * u_sdMatrices.ViewMat * u_modelMat * pos;
}

#version 430 core
#pragma stage : frag 
precision mediump float;

void main() {
	// Empty implementation for shadow map
}
