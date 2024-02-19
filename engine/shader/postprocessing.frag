#version 330 core
precision mediump float;

uniform sampler2D u_composite;

in vec2 v_texcoord;

out vec4 o_color;

void main() {
    vec4 color = texture(u_composite, v_texcoord);
    vec3 colorRGB = color.rgb;

    // HDR tonemapping
    colorRGB = colorRGB / (colorRGB + vec3(1.0));
    // gamma correct
    colorRGB = pow(colorRGB, vec3(1.0/2.2)); 

    o_color = vec4(colorRGB.rgb, color.a);
}
