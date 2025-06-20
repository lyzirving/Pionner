/*// --- Pionner Engine PostProcess Shader ---
// -------------------------------------------
*/
#version 430 core
#pragma stage : vert
precision mediump float;

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_Texcoord;

out v2f {
    vec2 v_TexCoord;
};

void main() {
	v_TexCoord = a_Texcoord;
	gl_Position = vec4(a_Pos, 1.f);
}

#version 430 core 
#pragma stage : frag 
precision mediump float;

#include "CommonGraphics.glslh"

uniform sampler2D u_ColorBuffer;

in v2f {
    vec2 v_TexCoord;
}; 

out vec4 o_FragColor;

void main() {
	vec4 base = texture(u_ColorBuffer, v_TexCoord);
	vec3 color = GammaCorrect(base.rgb);
    o_FragColor = vec4(color, base.a);
}