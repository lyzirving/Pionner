/*// -- Pionner Engine MainLightCaster Pass Shader --
// -------------------------------------------
// Filling main light's shadow map
*/
#version 430 core
#pragma stage : vert
precision mediump float;

#include "Motion.glslh"
#include "ShadowUniforms.glslh"

layout (location = 0) in vec3 a_Pos;
layout (location = 5) in vec4 a_Weights;
layout (location = 6) in ivec4 a_BoneIds;

void main() {
    gl_Position = u_DirLitShadowCaster.PrjMat * u_DirLitShadowCaster.ViewMat * GetPosition(a_Pos, a_BoneIds, a_Weights);
}

#version 430 core 
#pragma stage : frag 
precision mediump float;

void main() {
    // Empty implementation for shadow map
}