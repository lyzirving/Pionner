// specify the minimum version of opengl glsl, if current version is lower than this, the shader will not complie
#version 330 core

precision mediump float;

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex;


out vec2 v_pos;

void main() {
    v_pos.xy = a_pos.xy;
    gl_Position = vec4(a_pos, 1.0);
}