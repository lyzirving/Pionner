/*// -- Pionner Engine EquirectangularMap Shader --
// ----------------------------------------------
*/
#version 430 core
#pragma stage : vert
precision mediump float;

layout (location = 0) in vec3 a_Pos;

uniform mat4 u_PrjMat;
uniform mat4 u_ViewMat;

out v2f {
    vec3 v_WorldPos;
};

void main() {
	v_WorldPos = a_Pos;
    gl_Position =  u_PrjMat * u_ViewMat * vec4(a_Pos, 1.0);
}

#version 430 core 
#pragma stage : frag 
precision mediump float;

in v2f {
    vec3 v_WorldPos;
};

uniform sampler2D u_EquirectangularMap;

out vec4 o_FragColor;

vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	//inv atan
    uv *= vec2(0.1591, 0.3183);
    uv += 0.5;
    return uv;
}

void main() {
	vec2 uv = SampleSphericalMap(normalize(v_WorldPos));
    o_FragColor = vec4(texture(u_EquirectangularMap, uv).rgb, 1.0);
}