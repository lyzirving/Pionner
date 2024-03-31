#version 330 core

precision mediump float;

layout (location = 0) in vec3 a_pos;

uniform mat4 u_viewMat;
uniform mat4 u_prjMat;

out vec3 v_localPos;

void main()
{
    v_localPos = a_pos;
    gl_Position = u_prjMat * u_viewMat * vec4(a_pos, 1.0);
}