#version 330 core

precision mediump float;

in vec3 v_cubePos;
out vec4 o_color;

uniform samplerCube u_envMap;

void main()
{		
    vec3 envColor = texture(u_envMap, v_cubePos).rgb;

    // HDR tonemap and gamma correct
    envColor = envColor / (envColor + vec3(1.f));
    envColor = pow(envColor, vec3(1.f / 2.2f)); 
    
    o_color = vec4(envColor, 1.f);
}

