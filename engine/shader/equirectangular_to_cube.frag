#version 330 core

precision mediump float;

in vec3 v_cubePos;
out vec4 o_color;

uniform sampler2D u_equirectangularMap;

// 0.5 / PI = 0.1591
// 1.0 / PI = 0.3183
const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 sampleSphericalMap(vec3 v);

void main()
{		
    vec2 uv = sampleSphericalMap(normalize(v_cubePos));
    vec3 color = texture(u_equirectangularMap, uv).rgb;
    o_color = vec4(color, 1.0);
}

vec2 sampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}
