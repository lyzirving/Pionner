/*// -- Pionner Engine Geometry Pass Shader --
// -------------------------------------------
// Filling the G-Buffer
*/
#version 430 core
#pragma stage : vert
precision mediump float;

#include <Buffers.glslh>
#include <Normal.glslh>

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texcoord;
layout (location = 2) in vec3 a_normal;

uniform mat4 u_modelMat;

out VsOut {
    vec3 v_worldPos;
    vec3 v_normal;
    vec2 v_texCoord;
    flat mat3 v_TBN;
}; 

void main() {
    mat3 normalMat = NormalMat(u_modelMat);

    v_worldPos = vec3(u_modelMat * vec4(a_pos, 1.f));
    v_texCoord = a_texcoord;
    // Avoid matrix mutiply in fragment shader
    v_normal = normalMat * a_normal;
    v_TBN = mat3(1.f);

    mat4 prjMat = (u_camera.PrjType == 0) ? u_camera.PrjMat : u_camera.Ortho;
    gl_Position = prjMat * u_camera.ViewMat * u_modelMat * vec4(a_pos, 1.f);
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
layout (location = 0) out vec4 GPosition;
layout (location = 1) out vec4 GNormal;
layout (location = 2) out vec4 GAlbedoAlpha;
layout (location = 3) out vec4 GMaterial;// roughness(r) + metalness(g) + ao(b)
layout (location = 4) out vec4 GEmission;

void main() {
    vec4 albedoColor = texture(u_pbrMaterial.AlbedoTexture, v_texCoord);
    vec4 metallicRoughness = texture(u_pbrMaterial.MetallicRoughnessTexture, v_texCoord);

    GPosition = vec4(v_worldPos, 1.f);
    GNormal = normalize(vec4(v_normal, 1.f)); 

    GAlbedoAlpha = vec4(albedoColor.rgb * u_pbrMaterial.AlbedoColor, u_pbrMaterial.Alpha);
    // In metallic-roughness flow, g channel is roughness factor, b is metallic factor
    // Minimum roughness of 0.05 to keep specular highlight
    GMaterial.r = max(metallicRoughness.g * u_pbrMaterial.Roughness, 0.05);
    GMaterial.g = metallicRoughness.b * u_pbrMaterial.Metalness;
    GMaterial.b = 0.f;
    GMaterial.a = 1.f;

    GEmission = vec4(texture(u_pbrMaterial.EmissionTexture, v_texCoord).rgb * u_pbrMaterial.Emission, 1.f);
}