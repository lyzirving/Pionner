/*// -- Pionner Engine sprite shader --
// -------------------------------------
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

uniform sampler2D u_texture;

uniform bool u_bSRGB;
uniform bool u_bGammaCorrect;

in vec2 v_texcoord;
out vec4 o_color;

void main() {
    vec4 base = texture(u_texture, v_texcoord);
	vec3 rgbVal = u_bSRGB ? base.rgb : Mon2Lin(base.rgb);
	vec3 color = u_bGammaCorrect ? GammaCorrect(rgbVal) : rgbVal;
    o_color = vec4(color.rgb, base.a);
}