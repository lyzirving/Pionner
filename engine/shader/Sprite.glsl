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

out vec2 v_texcoord;

void main() {
	v_texcoord = a_texcoord;
	
	//TODO: deal with Orthographic projection
	gl_Position = u_camera.PrjMat * u_camera.ViewMat * u_modelMat * vec4(a_pos, 1.f);
}

#version 430 core 
#pragma stage : frag 
precision mediump float;

#include <Materials.glslh>

in vec2 v_texcoord; 
out vec4 o_color;

void main() {
	vec2 uv = vec2(u_spriteMaterial.FlipX ? (1.f - v_texcoord.x) : v_texcoord.x, u_spriteMaterial.FlipY ? (1.f - v_texcoord.y) : v_texcoord.y);
	o_color = texture(u_spriteMaterial.SpriteTexture, uv) * vec4(u_spriteMaterial.Color, 1.f);
}