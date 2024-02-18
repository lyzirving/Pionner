#version 330 core
precision mediump float;

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texcoord;

out vec2 v_texcoord; 

void main() {
	v_texcoord = a_texcoord;
	// TODO: use window coordinate??
	gl_Position = vec4(a_pos, 1.f);
}