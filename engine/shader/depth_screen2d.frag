// specify the minimum version of opengl glsl, if current version is lower than this, the shader will not complie
#version 330 core

precision mediump float;

uniform sampler2D u_depSampler;

in vec2 v_tex;

out vec4 o_color;

void main() {
     float depth = texture(u_depSampler, v_tex).r;
     o_color = vec4(vec3(depth), 1.f);
}
