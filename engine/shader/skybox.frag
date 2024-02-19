#version 330 core

precision mediump float;

in vec3 v_cubePos;
out vec4 o_color;

uniform samplerCube u_envMap;

void main()
{		
    o_color = vec4(texture(u_envMap, v_cubePos).rgb, 1.f);
}

