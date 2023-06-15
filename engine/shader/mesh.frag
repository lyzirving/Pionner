// specify the minimum version of opengl glsl, if current version is lower than this, the shader will not complie
#version 330 core

precision highp float;

struct Material {
    int  shadingModel; // 1 for constant color, 2 for ambient + diffuse, 3 for ambient + diffuse + specular
    int  texType;      // 1 for diffuse, 2 for specular
    int  hasTexture;   // 0 for no texture, otherwise this mesh has one texture
    vec3 ka, kd, ks;
    sampler2D diffuseTexture;
    sampler2D specTexture;
};

in vec3 v_pos;
in vec2 v_tex;

uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_prjMat;

uniform Material u_material;

out vec4 o_color;

float computeDepth(vec3 pos);

vec4  materialColor(Material material, vec2 texCoord);

void main() {
    o_color = materialColor(u_material, v_tex);

    gl_FragDepth = computeDepth(v_pos);
}

float computeDepth(vec3 pos)
{
    vec4 posClipSpace = u_prjMat * u_viewMat * u_modelMat * vec4(pos, 1.f);
    return (posClipSpace.z / posClipSpace.w);
}

vec4 materialColor(Material material, vec2 texCoord)
{
    vec4 color;
    bool hasTexture = material.hasTexture != 0;

    if(material.texType == 1)
    {
        color = hasTexture ? texture(material.diffuseTexture, texCoord) : vec4(material.kd, 1.f);
    }
    else
    {
        // in this case, if material does not have a texture, we also use kd.
        color = hasTexture ? texture(material.specTexture, texCoord) : vec4(material.kd, 1.f);
    }
    return color;
}
