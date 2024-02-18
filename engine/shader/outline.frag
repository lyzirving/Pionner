#version 330 core
precision mediump float;

uniform vec4 u_outlineColor;

in vec3 v_worldPos;
in vec3 v_normal;

out vec4 o_color;

void main() {
    o_color = u_outlineColor;
}
