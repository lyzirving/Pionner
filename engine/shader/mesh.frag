// specify the minimum version of opengl glsl, if current version is lower than this, the shader will not complie
#version 330 core

precision highp float;

const int NO_TEXTURE = 0;

struct Material {
    int  colorMode;    // mode3 = ambient + diffuse + specular
    int  hasAmbTex;    // 0 for no texture
    int  hasDiffTex;   // 0 for no texture
    int  hasSpecTex;   // 0 for no texture
    int  hasNormal;    // 0 for no texture
    vec3 ka, kd, ks;
    sampler2D ambTexture;
    sampler2D diffuseTexture;
    sampler2D specTexture;
    sampler2D normTexture;
};

in vec3 v_pos;
in vec2 v_tex;

uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_prjMat;

uniform Material u_material;

out vec4 o_color;

float computeDepth(vec3 pos);
vec4 objDiffColor(vec2 texCoord);

void main() {
    o_color = objDiffColor(v_tex);

    gl_FragDepth = computeDepth(v_pos);
}

float computeDepth(vec3 pos)
{
    vec4 posClipSpace = u_prjMat * u_viewMat * u_modelMat * vec4(pos, 1.f);
    return (posClipSpace.z / posClipSpace.w);
}

vec4 objDiffColor(vec2 texCoord)
{
    bool hasTexture = u_material.hasDiffTex != NO_TEXTURE;
    return hasTexture ? texture(u_material.diffuseTexture, texCoord) : vec4(u_material.kd, 1.f);
}
