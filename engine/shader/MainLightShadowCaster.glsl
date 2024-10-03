/*// -- Pionner Engine MainLightCaster Pass Shader --
// -------------------------------------------
// Filling main light's shadow map
*/
#version 430 core
#pragma stage : vert
precision mediump float;

#include <Lighting.glslh>

layout (location = 0) in vec3 a_pos;

uniform int u_lightTech;
uniform int u_cascadeIdx;

uniform mat4 u_modelMat;

void main() {
    mat4 viewMat = mat4(0.f);
    mat4 prjMat = mat4(0.f);

    viewMat += when_eqi(u_lightTech, LIGHT_TECH_SHADOW_MAP) * u_mainLightCaster.ViewMat;
    viewMat += when_eqi(u_lightTech, LIGHT_TECH_CASCADE_SHADOW_MAP) * u_CSM.ViewMat;

    prjMat += when_eqi(u_lightTech, LIGHT_TECH_SHADOW_MAP) * u_mainLightCaster.PrjMat;
    prjMat += when_eqi(u_lightTech, LIGHT_TECH_CASCADE_SHADOW_MAP) * ((u_lightTech == LIGHT_TECH_CASCADE_SHADOW_MAP) ? u_CSM.PrjMats[u_cascadeIdx] : mat4(0.f));

    gl_Position = prjMat * viewMat * u_modelMat * vec4(a_pos, 1.f);
}

#version 430 core 
#pragma stage : frag 
precision mediump float;

void main() {
    // Empty implementation for shadow map
}