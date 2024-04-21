#version 330 core
precision mediump float;

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texcoord;

layout (std140) uniform Matrices
{
	mat4 ViewMat;
	mat4 PrjMat;
	mat4 Ortho;
    vec3 CameraPosition;
    float FrustumFar;
} u_matrices;

uniform mat4 u_modelMat; 

void main() {
	gl_Position = u_matrices.PrjMat * u_matrices.ViewMat * u_modelMat * vec4(a_pos, 1.f);
}