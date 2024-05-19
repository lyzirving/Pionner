#version 330 core
precision mediump float;

layout (std140) uniform Matrices
{
	mat4 ViewMat;
	mat4 PrjMat;
	mat4 Ortho;
    vec3 CameraPosition;
    float FrustumFar;
} u_matrices;

struct MaterialAttributes 
{
    vec3 AlbedoColor;
    vec3 Emission;
    float Roughness;
    float Metalness;
    float AO;
    float Alpha;
    bool UseNormalMap;

    sampler2D AlbedoTexture;
    sampler2D EmissionTexture;
    sampler2D MetallicRoughnessTexture;
    sampler2D NormalTexture;
    sampler2D AOTexture;
};

uniform MaterialAttributes u_material;

in VsOut {
    vec3 v_worldPos;
    vec3 v_normal;
    vec2 v_texCoord;
    flat mat3 v_TBN;
}; 

out vec4 o_color;

void main() {
    vec4 albedoColor = texture(u_material.AlbedoTexture, v_texCoord);
    o_color = vec4(albedoColor.rgb * u_material.AlbedoColor.rgb, albedoColor.a);
}
