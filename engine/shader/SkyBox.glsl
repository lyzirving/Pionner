/*// -- Pionner Engine Sky Box Shader --
// -------------------------------------
*/
#version 430 core
#pragma stage : vert
precision mediump float;

#include <Buffers.glslh>

layout (location = 0) in vec3 a_pos;

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

#version 430 core 
#pragma stage : frag 
precision mediump float;

#include <Common.glslh>

in vec3 v_cubePos;
out vec4 o_color;

uniform samplerCube u_envMap;

void main()
{
    o_color = vec4(HdrToneMapping(texture(u_envMap, v_cubePos).rgb), 1.0);
}