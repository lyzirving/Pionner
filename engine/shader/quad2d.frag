#version 330 core
precision mediump float;

uniform sampler2D u_quadTexture;

in vec2 v_texcoord;

out vec4 o_color;

void main() {
    o_color = texture(u_quadTexture, v_texcoord);
}
