/*// -- Pionner Engine Sprite Rendering Shader --
// ----------------------------------------------
*/
#version 430 core
#pragma stage : vert
precision mediump float;

#include <Buffers.glslh>

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texcoord;
layout (location = 2) in vec3 a_normal;

uniform mat4 u_modelMat;
uniform bool u_bOverlay;

out vec2 v_texcoord;

void main() {
	v_texcoord = a_texcoord;

	mat4 prjMat = (u_bOverlay) ? u_camera.Ortho : u_camera.PrjMat;
	gl_Position = prjMat * u_camera.ViewMat * u_modelMat * vec4(a_pos, 1.f);
}

#version 430 core 
#pragma stage : frag 
precision mediump float;

#include <Materials.glslh>

in vec2 v_texcoord; 
out vec4 o_color;

void main() {
	o_color = texture(u_spriteMaterial.SpriteTexture, v_texcoord);
}