/*// -- Pionner Engine ColorCorrect Pass Shader --
// -------------------------------------------
*/
#version 430 core
#pragma stage : vert
precision mediump float;

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texcoord;

out vec2 v_texcoord;

void main() {
	v_texcoord = a_texcoord;
	gl_Position = vec4(a_pos, 1.f);
}

#version 430 core 
#pragma stage : frag 
precision mediump float;

#include <Common.glslh>

uniform sampler2D u_colorBuffer;

in vec2 v_texcoord; 
out vec4 o_color;

void main() {
	vec4 base = texture(u_colorBuffer, v_texcoord);
	vec3 color = GammaCorrect(base.rgb);
    o_color = vec4(color, base.a);
}