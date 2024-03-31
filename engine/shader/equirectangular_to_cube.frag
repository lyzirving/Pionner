#version 330 core
precision mediump float;

// 0.5 / PI = 0.1591
// 1.0 / PI = 0.3183
const vec2 invAtan = vec2(0.1591, 0.3183);

uniform sampler2D u_equirectangularMap;

vec2 sampleSphericalMap(vec3 v);

in vec3 v_localPos;
out vec4 o_color;

void main()
{		
    vec2 uv = sampleSphericalMap(normalize(v_localPos));
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
