#version 330 core
precision mediump float;

uniform vec4 u_color;

out vec4 o_fragColor;

void main() {
    o_fragColor = u_color;
}
