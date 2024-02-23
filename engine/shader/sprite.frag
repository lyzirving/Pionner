#version 330 core
precision mediump float;

const float Gamma = 2.2f;

uniform sampler2D u_quadTexture;
uniform bool u_bSRGB;

in vec2 v_texcoord;

out vec4 o_color;

vec4 toLinear(vec4 sRGB);

void main() {
    vec4 baseColor = texture(u_quadTexture, v_texcoord);
    // TODO: why mutiple 255
    o_color = u_bSRGB ? toLinear(baseColor * 255.f) : baseColor;
}

vec4 toLinear(vec4 sRGB)
{
    bvec4 cutoff = lessThan(sRGB, vec4(0.04045));
    vec4 higher = pow((sRGB + vec4(0.055)) / vec4(1.055), vec4(2.4));
    vec4 lower = sRGB / vec4(12.92);
    return mix(higher, lower, cutoff);
}
