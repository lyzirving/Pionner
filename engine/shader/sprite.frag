#version 330 core
precision mediump float;

const float Gamma = 2.2f;

uniform sampler2D u_quadTexture;
uniform bool u_bSRGB;

in vec2 v_texcoord;

out vec4 o_color;

vec3 toLinear(vec3 sRGB);

void main() {
    vec4 baseColor = texture(u_quadTexture, v_texcoord);
    // TODO: why mutiple 255, is baseColor in [0, 1] ?
    vec3 color = u_bSRGB ? toLinear(baseColor.rgb * 255.f) : baseColor.rgb;
    o_color = vec4(color, baseColor.a);
}

vec3 toLinear(vec3 sRGB)
{
    bvec3 cutoff = lessThan(sRGB, vec3(0.04045));
    vec3 higher = pow((sRGB + vec3(0.055)) / vec3(1.055), vec3(2.4));
    vec3 lower = sRGB / vec3(12.92);
    return mix(higher, lower, cutoff);
}
