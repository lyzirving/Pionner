#version 330 core
precision mediump float;

const float Gamma = 2.2f;

uniform sampler2D u_quadTexture;
uniform bool u_bSRGB;

in vec2 v_texcoord;

out vec4 o_color;

vec3 mon2lin(vec3 x);

void main() {
    vec4 baseColor = texture(u_quadTexture, v_texcoord);    
    vec3 color = u_bSRGB ? baseColor.rgb : mon2lin(baseColor.rgb);
    o_color = vec4(color, baseColor.a);
}

vec3 mon2lin(vec3 x)
{
    return vec3(pow(x[0], 2.2), pow(x[1], 2.2), pow(x[2], 2.2));
}
