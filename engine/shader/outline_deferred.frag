#version 330 core
precision mediump float;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec4 gNormal;      // normal(3) + type(1)
layout (location = 2) out vec4 gAlbedoAlpha;
layout (location = 3) out vec3 gMaterial;    // roughness + metalness + ao
layout (location = 4) out vec3 gEmission;    //

uniform vec4 u_outlineColor;

in vec3 v_worldPos;
in vec3 v_normal;

void main() {
	gPosition = v_worldPos;
    // gNormal.w is type component, 3 for outline
    gNormal = vec4(v_normal, 3.f);
    gAlbedoAlpha = u_outlineColor;
    gMaterial = vec3(0.5f, 0.f, 1.f);
    gEmission = vec3(0.f);
}
