/*// -- Pionner Engine MainLightCaster Pass Shader --
// -------------------------------------------
// Filling main light's shadow map
*/
#version 430 core
#pragma stage : vert
precision mediump float;

#include <Lighting.glslh>

layout (location = 0) in vec3 a_pos;

uniform mat4 u_modelMat;

void main() {
    gl_Position = u_mainLightCaster.PrjMat * u_mainLightCaster.ViewMat * u_modelMat * vec4(a_pos, 1.f);
}

#version 430 core 
#pragma stage : frag 
precision mediump float;

void main() {
    // Empty implementation for shadow map
}