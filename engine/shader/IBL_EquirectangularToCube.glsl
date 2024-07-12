/*// -- Pionner Engine Equirectangular Map To CubeMap Shader --
// ------------------------------------------------------------
*/
#version 430 core
#pragma stage : vert
precision mediump float;

layout (location = 0) in vec3 a_pos;

uniform mat4 u_viewMat;
uniform mat4 u_prjMat;

out vec3 v_localPos;

void main()
{
    v_localPos  = a_pos;
    gl_Position = u_prjMat * u_viewMat * vec4(a_pos, 1.0);
}

#version 430 core 
#pragma stage : frag 
precision mediump float;

#include <IBL.glslh>

uniform sampler2D u_equirectangularMap;

in vec3  v_localPos;
out vec4 o_color;

void main()
{		
    vec2 uv = SampleSphericalMap(normalize(v_localPos));
    vec3 color = texture(u_equirectangularMap, uv).rgb;
    o_color = vec4(color, 1.0);
}