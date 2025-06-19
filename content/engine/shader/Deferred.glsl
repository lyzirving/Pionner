/*// -- Pionner Engine Deferred Pass Shader --
// -------------------------------------------
// Filling the Deferred pass's frame buffer
*/
#version 430 core
#pragma stage : vert
precision highp float;

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_Texcoord;

out v2f {
    vec2 v_TexCoord;
}; 

void main() {
	v_TexCoord = a_Texcoord;
	gl_Position = vec4(a_Pos, 1.f);
}

#version 430 core 
#pragma stage : frag 
precision highp float;

uniform sampler2D u_GPosition;   // vec3
uniform sampler2D u_GNormal;     // vec4 noraml(3) + type(1)
uniform sampler2D u_GAlbedoAlpha;// vec4 albedo(3) + alpha(1)
uniform sampler2D u_GMaterial;   // vec4 ao(r) + roughness(g) + metalness(b) + clearCoat(a)
uniform sampler2D u_GMaterialSub;// vec4 emission(rgb) + clearCoatRoughness(a)

in v2f {
    vec2 v_TexCoord;
}; 

#include "SurfaceShading.glslh"

void MakeMaterial(inout MaterialInputs material);
void PrepareShading();

out vec4 o_FragColor;

void main() {
	MaterialInputs material;
    InitMaterial(material);
    MakeMaterial(material);
    PrepareShading();

    vec3 color = EvaluateMaterial(material);
    o_FragColor = vec4(color.rgb, material.baseColor.a);
}

void MakeMaterial(inout MaterialInputs material) {		
	vec4 surface = texture(u_GMaterial, v_TexCoord);
    vec4 surfaceSub = texture(u_GMaterialSub, v_TexCoord);
	material.baseColor = texture(u_GAlbedoAlpha, v_TexCoord);
	material.roughness = surface.g;
    material.metallic = surface.b;
    material.emissive = vec4(surfaceSub.rgb, 1.0);
    material.clearCoat = surface.a;
    material.clearCoatRoughness = surfaceSub.a;
}

void PrepareShading() {
    m_ShadingPosition = texture(u_GPosition, v_TexCoord).xyz;
	m_ShadingNormal = texture(u_GNormal, v_TexCoord).xyz;
    m_ShadingView = normalize(u_Camera.Position - m_ShadingPosition);
    m_ShadingReflected = reflect(-m_ShadingView, m_ShadingNormal);
    m_ShadingNoV = max(dot(m_ShadingNormal, m_ShadingView), MIN_N_DOT_V);
}