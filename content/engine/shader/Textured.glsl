/*// -- Pionner Engine Textured Rendering Shader --
// ----------------------------------------------
*/
#version 430 core
#pragma stage : vert
precision mediump float;

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_Texcoord;

#include "Camera.glslh"
#include "Motion.glslh"

out v2f {
    vec2 v_TexCoord;
};

void main() {
	v_TexCoord = a_Texcoord;	
	gl_Position = u_Camera.PrjMat * u_Camera.ViewMat * u_Motion.Transform * u_Motion.LocalTransform * vec4(a_Pos, 1.f);
}

#version 430 core 
#pragma stage : frag 
precision mediump float;

#include "material/TexturedMaterial.glslh"

in v2f {
    vec2 v_TexCoord;
};

out vec4 o_FragColor;

void main() {
	vec2 uv;
	uv.x = v_TexCoord.x;
	uv.y = u_TexturedMaterial.FlipY ? (1.0 - v_TexCoord.y) : v_TexCoord.y;
	uv = uv * u_TexturedMaterial.Tile * u_TexturedMaterial.Scale + u_TexturedMaterial.Offset;
	vec4 color = texture(u_Texture, uv);
	o_FragColor = vec4(color.rgb * u_TexturedMaterial.Color, color.a * u_TexturedMaterial.Alpha);
}