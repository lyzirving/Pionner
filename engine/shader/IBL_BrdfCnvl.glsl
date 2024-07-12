/*// -- Pionner Engine Brdf Convolution Shader --
// ------------------------------------------------------------
*/
#version 430 core
#pragma stage : vert
precision mediump float;

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texcoord;

out vec2 v_texcoord; 

void main() {
	v_texcoord = a_texcoord;
	// TODO: use window coordinate??
	gl_Position = vec4(a_pos, 1.f);
}

#version 430 core 
#pragma stage : frag 
precision mediump float;

#include <IBL.glslh>

in vec2  v_texcoord;
out vec4 o_fragColor;

void main() {
    vec2 integratedBRDF = IntegrateBRDF(v_texcoord.x, v_texcoord.y);
    o_fragColor = vec4(integratedBRDF.x, integratedBRDF.y, 0.0, 1.f);
}