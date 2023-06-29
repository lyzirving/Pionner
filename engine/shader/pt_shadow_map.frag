// specify the minimum version of opengl glsl, if current version is lower than this, the shader will not complie
#version 330 core

precision highp float;

uniform vec3  u_lightPos;
uniform float u_farPlane;

in vec4 v_fragPos;

void main() {
    float lightDist = length(v_fragPos.xyz - u_lightPos);

    // Map to [0;1] range by dividing by far_plane
    lightDist = lightDist / u_farPlane;

    // Write this as modified depth
    gl_FragDepth = lightDist;
}
