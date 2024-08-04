/*// -- Pionner Engine Lighting pass shader in deffered shading mode --
// --------------------------------------------------------------------
// Drawing a 2d quad
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

uniform sampler2D        u_shadowMap;
uniform samplerCubeArray u_ptShadowMap;

uniform sampler2D   u_brdfLUT;
uniform samplerCube u_irradianceMap;
uniform samplerCube u_prefilterMap;

uniform float u_envMapIntensity;
uniform vec4  u_bgColor;

in vec2  v_texcoord; 
out vec4 o_color;

vec4 LightingMeshColor()
{
    vec3 lightContribution = vec3(0.f);

    CalculateAttrs();
    lightContribution += (CalculateDirLightsEffect() * (u_distantLight.CastShadow ? (1.f - CalcDirLightShadow(u_distantLight.SdMode, u_shadowMap)) : 1.f));
    //[NOTE]: No point light shadow for current impl
    lightContribution += CalculatePointLightsEffect();

    vec3 iblContribution = IBL(u_brdfLUT, u_irradianceMap, u_prefilterMap) * u_envMapIntensity * m_PBRParams.AO;
    lightContribution += iblContribution;
    lightContribution += (m_PBRParams.Albedo * m_PBRParams.Emission);

    return vec4(lightContribution.rgb, m_PBRParams.Alpha);
}

void main() {
	vec4 baseColor = texture(u_GAlbedoAlpha, v_texcoord);
    vec4 nt = texture(u_GNormal, v_texcoord);// normal and type
    vec3 surface = texture(u_GMaterial, v_texcoord).rgb;

	m_PBRParams.FragPos = texture(u_GPosition, v_texcoord).rgb;
    m_PBRParams.Albedo = Mon2Lin(baseColor.rgb);
    m_PBRParams.Alpha = baseColor.a;
    m_PBRParams.N = nt.xyz;
    m_PBRParams.Roughness = surface.r;
    m_PBRParams.Metalness = surface.g;
    m_PBRParams.AO = surface.b;
    m_PBRParams.Emission = texture(u_GEmission, v_texcoord).rgb;

    int type = NeInt(nt.w);    
    o_color = (type == FRAG_TYPE_MESH) ? LightingMeshColor() : ((type == FRAG_TYPE_OUTLINE) ? baseColor : vec4(Mon2Lin(u_bgColor.rgb), u_bgColor.a));
}
