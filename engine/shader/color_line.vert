#version 330 core
precision mediump float;

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec4 a_color;

layout (std140) uniform Matrices
{
	mat4 ViewMat;
	mat4 PrjMat;
	mat4 Ortho;
    vec3 CameraPosition;
    float FrustumFar;
} u_matrices;

flat out vec4 v_color; 

void main() {
	v_color = a_color;
	gl_Position = u_matrices.PrjMat * u_matrices.ViewMat * vec4(a_pos, 1.f);
}