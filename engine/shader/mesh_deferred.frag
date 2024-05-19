#version 330 core
precision mediump float;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec4 gNormal;      // normal(3) + type(1)
layout (location = 2) out vec4 gAlbedoAlpha;
layout (location = 3) out vec3 gMaterial;    // roughness + metalness + ao
layout (location = 4) out vec3 gEmission;    //

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

in VsOut {
    vec3 v_worldPos;
    vec3 v_normal;
    vec2 v_texCoord;
    flat mat3 v_TBN;
}; 

uniform MaterialAttributes u_material;
uniform bool u_useNormalMap;

vec3 calcNormal();

void main() {
    vec4 albedoColor = texture(u_material.AlbedoTexture, v_texCoord);

    gPosition = v_worldPos;
    // w is type component, 2 for mesh
    gNormal = vec4(calcNormal(), 2.f);
    gAlbedoAlpha = vec4(albedoColor.rgb * u_material.AlbedoColor, albedoColor.a);
    gMaterial.x = texture(u_material.MetallicRoughnessTexture, v_texCoord).g * u_material.Roughness;
    gMaterial.x = max(gMaterial.x, 0.05f);// Minimum roughness of 0.05 to keep specular highlight
    gMaterial.y = texture(u_material.MetallicRoughnessTexture, v_texCoord).r * u_material.Metalness;
    gMaterial.z = texture(u_material.AOTexture, v_texCoord).r * u_material.AO;
    gEmission = texture(u_material.EmissionTexture, v_texCoord).rgb * u_material.Emission;
}

vec3 calcNormal()
{
    vec3 N;
    if(u_useNormalMap)
    {
        N = texture(u_material.NormalTexture, v_texCoord).rgb;
        // turn normal from [0, 1] into [-1, 1]
        // now normal is in tangent space
        N = 2.f * N - 1.f;
    }
    else
    {
        N = v_normal;
    }
    return normalize(v_TBN * N);
}
