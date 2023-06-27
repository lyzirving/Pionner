// specify the minimum version of opengl glsl, if current version is lower than this, the shader will not complie
#version 330 core

precision mediump float;

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex;

uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_prjMat;

out vec3 v_pos;
out vec3 v_normal;
out vec2 v_tex;

void main() {
    v_pos = a_pos;
    v_normal = a_normal;
    v_tex = a_tex;

    gl_Position = u_prjMat * u_viewMat * u_modelMat * vec4(a_pos, 1.0);
}