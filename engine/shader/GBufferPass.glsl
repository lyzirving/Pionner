/*// -- Pionner Engine Geometry Pass Shader --
// -------------------------------------------
// Filling the G-Buffer
*/
#version 430 core
#pragma stage : vert
precision mediump float;

#include <Buffers.glslh>

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texcoord;

uniform mat4 u_modelMat;

out VsOut {
    vec3 v_worldPos;
    vec3 v_normal;
    vec2 v_texCoord;
    flat mat3 v_TBN;
}; 

void main() {
    v_worldPos = vec3(u_modelMat * vec4(a_pos, 1.f));
    v_texCoord = a_texcoord;
    v_normal = a_normal;
    v_TBN = mat3(1.f);

    gl_Position = u_camera.PrjMat * u_camera.ViewMat * u_modelMat * vec4(a_pos, 1.f);
}

#version 430 core 
#pragma stage : frag 
precision mediump float;

#include <Materials.glslh>

in VsOut {
    vec3 v_worldPos;
    vec3 v_normal;
    vec2 v_texCoord;
    flat mat3 v_TBN;
}; 

// Different render target for G-Buffer
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;      // normal(3) + type(1)
layout (location = 2) out vec4 gAlbedoAlpha;
layout (location = 3) out vec4 gMaterial;    // roughness + metalness + ao
layout (location = 4) out vec4 gEmission;

void main() {
    vec4 albedoColor = texture(u_pbrMaterial.AlbedoTexture, v_texCoord);

    gPosition = vec4(v_worldPos, 1.f);
    gNormal = vec4(v_normal, 1.f); 
    gAlbedoAlpha = vec4(albedoColor.rgb * u_pbrMaterial.AlbedoColor, albedoColor.a);
    gMaterial = vec4(1.f);
    gEmission = vec4(0.f);
}