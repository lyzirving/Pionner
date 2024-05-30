#version 430 core
#pragma stage : vert

#include <PBR.glslh>

layout (location = 0) in vec3  a_pos;

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

#version 430 core 
#pragma stage : frag 

out vec4 o_color;

void main() {
    o_color = vec4(1.f, 0.f, 0.f, 1.f);
}