// specify the minimum version of opengl glsl, if current version is lower than this, the shader will not complie
#version 330 core

precision mediump float;

layout (location = 0) in vec3 a_pos;

uniform mat4 u_modelMat;
uniform mat4 u_lightViewMat;
uniform mat4 u_lightPrjMat;

void main()
{
    gl_Position = u_lightPrjMat * u_lightViewMat * u_modelMat * vec4(a_pos, 1.0);
}