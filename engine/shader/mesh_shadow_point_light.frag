#version 330 core

precision highp float;

in vec4 v_fragPos;
flat in vec3 v_lightPos;
flat in float v_frustumFar;

void main() {
    float lightDist = length(v_fragPos.xyz - v_lightPos);

    // Map to [0;1] range by dividing by far_plane
    lightDist = lightDist / v_frustumFar;

    // Write this as modified depth
    gl_FragDepth = lightDist;
}
