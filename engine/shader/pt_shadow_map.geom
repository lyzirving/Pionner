// specify the minimum version of opengl glsl, if current version is lower than this, the shader will not complie
#version 330 core

precision highp float;

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 u_lightMats[6];

out vec4 v_fragPos; // v_fragPos from geometry shader (output per emitvertex())

void main() {
    for(int face = 0; face < 6; ++face)
    {
        // Built-in variable that specifies to which face we render.
        // This is mainly for cube texture.
        gl_Layer = face; 
        for(int i = 0; i < 3; ++i) // For each triangle's vertices
        {
            v_fragPos = gl_in[i].gl_Position;
            gl_Position = u_lightMats[face] * v_fragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}
