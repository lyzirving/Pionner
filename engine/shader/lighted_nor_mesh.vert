// specify the minimum version of opengl glsl, if current version is lower than this, the shader will not complie
#version 330 core

precision mediump float;

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex;
layout (location = 3) in vec3 a_tangents;

uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_prjMat;

uniform mat3 u_normalMat;

uniform vec3 u_viewPos;
uniform vec3 u_lightPos;
uniform vec3 u_lightDir;

out VS_OUT {
    vec3 fragPos;
    vec2 texCoords;
    vec3 tangentLightPos;
    vec3 tangentLightDir;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
} vs_out;

void main() {
    vec3 T = normalize(u_normalMat * a_tangents);
    vec3 N = normalize(u_normalMat * a_normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));

    vs_out.fragPos = vec3(u_modelMat * vec4(a_pos, 1.f));
    vs_out.texCoords = a_tex;
    vs_out.tangentLightPos = TBN * u_lightPos;
    vs_out.tangentLightDir = TBN * u_lightDir;
    vs_out.tangentViewPos  = TBN * u_viewPos;
    vs_out.tangentFragPos  = TBN * vs_out.fragPos;

    gl_Position = u_prjMat * u_viewMat * u_modelMat * vec4(a_pos, 1.0);
}