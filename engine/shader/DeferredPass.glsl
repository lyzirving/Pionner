/*// -- Pionner Engine Deferred Pass Shader --
// -------------------------------------------
// Filling the Deferred pass's frame buffer
*/
#version 430 core
#pragma stage : vert
precision mediump float;

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texcoord;

out vec2 v_texcoord;

void main() {
	v_texcoord = a_texcoord;
	gl_Position = vec4(a_pos, 1.f);
}

#version 430 core 
#pragma stage : frag 
precision mediump float;

#include <Lighting.glslh>

uniform sampler2D u_GPosition;   // vec3
uniform sampler2D u_GNormal;     // vec4 noraml(3) + type(1)
uniform sampler2D u_GAlbedoAlpha;// vec4 albedo(3) + alpha(1)
uniform sampler2D u_GMaterial;   // vec3 roughness + metalness + ao
uniform sampler2D u_GEmission;   // vec3

in vec2 v_texcoord; 
out vec4 o_color;

vec4 LightContribution();

void main() {
	vec4 baseColor = texture(u_GAlbedoAlpha, v_texcoord);	
	vec3 surface = texture(u_GMaterial, v_texcoord).rgb;

	m_PBRParams.FragPos = texture(u_GPosition, v_texcoord).xyz;
    m_PBRParams.Albedo = baseColor.rgb;
    m_PBRParams.Alpha = baseColor.a;
    m_PBRParams.N = texture(u_GNormal, v_texcoord).xyz;
    m_PBRParams.Roughness = surface.r;
    m_PBRParams.Metalness = surface.g;
    m_PBRParams.Emission = texture(u_GEmission, v_texcoord).rgb;

	m_PBRParams.V = normalize(u_camera.Position - m_PBRParams.FragPos);
    m_PBRParams.R = reflect(-m_PBRParams.V, m_PBRParams.N);
    m_PBRParams.NdotV = max(dot(m_PBRParams.N, m_PBRParams.V), 0.f);
    m_PBRParams.F0 = mix(U_F0, m_PBRParams.Albedo, m_PBRParams.Metalness);

	o_color = LightContribution();
}

vec4 LightContribution()
{
    vec3 lightContribution = vec3(0.f);
    lightContribution += CalculateDirLightsEffect();
    return vec4(lightContribution, m_PBRParams.Alpha);
}