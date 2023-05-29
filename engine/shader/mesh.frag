// specify the minimum version of opengl glsl, if current version is lower than this, the shader will not complie
#version 330 core

precision highp float;

struct Light {
    int   type; // 1 for directional light, 2 for point light
    vec3  pos;  // light's position in world coordinate system
    vec3  direction;
    vec3  ka, kd, ks; // color of ambient, diffuse, specular
    float ia, id, is; // intensity of ambient, diffuse, specular
    float shininess;
    float attParamConst, attParamLinear, attParamQuad; // params of attenuation for point light
};

struct Material {
    int  shadingModel; // 1 for constant color, 2 for ambient + diffuse, 3 for ambient + diffuse + specular
    int  texType;      // 1 for diffuse, 2 for specular
    int  hasTexture;   // 0 for no texture, otherwise this mesh has one texture
    vec3 ka, kd, ks;
    sampler2D u_diffuse;
    sampler2D u_spec;
};

in vec3 v_pos;
in vec3 v_normal;
in vec2 v_tex;

uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_prjMat;
uniform mat4 u_normalMat;

uniform Light    u_light;
uniform Material u_material;

out vec4 o_color;

float computeDepth(vec3 pos);
float computeAttenuation(Light light, float dist);

vec4  materialColor(Material material, vec2 texCoord);
vec4  lightedSurface(Light light, Material material, vec3 pos, vec3 normal, vec2 texCoord);

void main() {
    vec3 fragPos = vec3(u_modelMat * vec4(v_pos, 1.f));
    vec3 normal  = normalize(vec3(u_normalMat * vec4(v_normal, 0.f)));
    vec2 texCoord = v_tex;

    o_color = lightedSurface(u_light, u_material, fragPos, normal, texCoord);

    // calculate depth for current position
    gl_FragDepth = computeDepth(v_pos);
}

float computeDepth(vec3 pos)
{
    vec4 posClipSpace = u_prjMat * u_viewMat * u_modelMat * vec4(pos, 1.f);
    return (posClipSpace.z / posClipSpace.w);
}

float computeAttenuation(Light light, float dist)
{
    return 1.f / (light.attParamConst + light.attParamLinear * dist + light.attParamQuad * (dist * dist));
}

vec4 materialColor(Material material, vec2 texCoord)
{
    vec4 color;
    bool hasTexture = material.hasTexture != 0;

    if(material.texType == 1)
    {
        color = hasTexture ? texture(material.u_diffuse, texCoord) : vec4(material.kd, 1.f);
    }
    else
    {
        // in this case, if material does not have a texture, we also use kd.
        color = hasTexture ? texture(material.u_spec, texCoord) : vec4(material.kd, 1.f);
    }
    return color;
}

vec4 lightedSurface(Light light, Material material, vec3 pos, vec3 normal, vec2 texCoord)
{
    vec4 surface;

    vec4 la = vec4(light.ka * light.ia, 1.f); // light's ambient 
    vec4 ld = vec4(light.kd * light.id, 1.f); // light's diffuse
    vec4 ls = vec4(light.ks * light.is, 1.f); // light's specular

    vec4 matColor = materialColor(material, texCoord);

    vec3 lightDir;

    if(light.type == 1) // directional light
    {
        lightDir = normalize(light.direction);
    } 
    else if(light.type == 2) // point light
    {
        lightDir = normalize(light.pos - pos);
        float dist = length(light.pos - pos);
        float attenu = computeAttenuation(light, dist);

        if(material.shadingModel == 1) // constant color model
        {
            surface = ld * matColor * attenu;
        }
        else if(material.shadingModel == 2) // diffuse model
        {
            vec4 ambient = la * matColor * attenu;

            float diff = max(dot(normal, lightDir), 0.f);
            vec4 diffuse = ld * diff * matColor * attenu;

            surface = ambient + diffuse;
        }
        else if(material.shadingModel == 3) // mixed model
        {
            vec4 ambient = la * matColor * attenu;

            float diff = max(dot(normal, lightDir), 0.f);
            vec4 diffuse = ld * diff * matColor * attenu;

            surface = ambient + diffuse;
        }
    }

    return surface;
}
