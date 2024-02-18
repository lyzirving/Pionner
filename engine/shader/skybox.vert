#version 330 core

precision mediump float;

layout (location = 0) in vec3 a_pos;

layout (std140) uniform Matrices
{
	mat4 ViewMat;
	mat4 PrjMat;
	mat4 Ortho;
    vec3 CameraPosition;
    float FrustumFar;
} u_matrices;

out vec3 v_cubePos;

void main()
{
    v_cubePos = a_pos;
    // remove translation from camera's view matrix
    mat4 rotMat = mat4(mat3(u_matrices.ViewMat));
    vec4 clipPos = u_matrices.PrjMat * rotMat * vec4(a_pos, 1.0);
    // after perspective division, the z(depth) is always 1.0
    // Note that 1.0 is the max depth
    gl_Position = clipPos.xyww;
}