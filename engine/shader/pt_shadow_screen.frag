// specify the minimum version of opengl glsl, if current version is lower than this, the shader will not complie
#version 330 core

precision highp float;

uniform samplerCube u_pointDepthMap;
uniform float       u_farPlane;

in vec3 v_fragPos;

out vec4 o_color;

void main() {
    float depth = texture(u_pointDepthMap, normalize(v_fragPos)).r;
    depth /= u_farPlane;

    o_color = vec4(depth, depth, depth, 1.f);
}
