/*// --- Pionner Engine Line Shader ----
// -------------------------------------
*/
#version 430 core
#pragma stage : vert
precision mediump float;

#include <Buffers.glslh>

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texcoord;

uniform mat4 u_modelMat; 

void main() {
	gl_Position = u_matrices.PrjMat * u_matrices.ViewMat * u_modelMat * vec4(a_pos, 1.f);
}

#version 430 core 
#pragma stage : frag 
precision mediump float;

uniform vec4 u_color;

out vec4 o_fragColor;

void main() {
    o_fragColor = u_color;
}