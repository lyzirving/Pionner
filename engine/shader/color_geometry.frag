// specify the minimum version of opengl glsl, if current version is lower than this, the shader will not complie
#version 330 core

precision highp float;

uniform vec4 u_color;
uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_prjMat;

in vec3 v_pos;

out vec4 o_color;

float computeDepth(vec3 pos);

void main() {
    o_color = u_color;
    gl_FragDepth = computeDepth(v_pos);
}

float computeDepth(vec3 pos)
{
    vec4 posClipSpace = u_prjMat * u_viewMat * u_modelMat * vec4(pos, 1.f);
    return (posClipSpace.z / posClipSpace.w);
}
