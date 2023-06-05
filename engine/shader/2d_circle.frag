// specify the minimum version of opengl glsl, if current version is lower than this, the shader will not complie
#version 330 core

precision highp float;

uniform vec4  u_color;
uniform vec2  u_viewportSize;
uniform float u_radius;

in vec2 v_pos;

out vec4 o_color;

void main() {
    vec2 uv = (v_pos + 1.f) * 0.5f;
    uv = uv * u_viewportSize;

    vec2 center = u_viewportSize / 2.f;

    float dist = length(center - uv);

    if(dist < u_radius)
        o_color = u_color;
    else
        o_color = vec4(0.f, 0.f, 0.f, 0.f);
}
