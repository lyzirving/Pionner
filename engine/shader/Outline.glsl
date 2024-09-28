/*// -- Pionner Engine Outline Rendering Shader --
// ----------------------------------------------
*/
#version 430 core
#pragma stage : vert
precision mediump float;

#include <Buffers.glslh>

layout (location = 0) in vec3 a_pos;

uniform mat4 u_modelMat;

void main() {
	gl_Position = u_camera.PrjMat * u_camera.ViewMat * u_modelMat * vec4(a_pos, 1.f);
}

#version 430 core 
#pragma stage : frag 
precision mediump float;

#include <Materials.glslh>

out vec4 o_color;

void main() {
	o_color = vec4(u_colorMaterial.Color, 1.f);
}