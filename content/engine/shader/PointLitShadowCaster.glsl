/*// -- Pionner Engine Point Light Shadow Cast shader --
// -----------------------------------------------------
// Filling the shadow map for all point lights in group
*/

#version 430 core
#pragma stage : vert
precision mediump float;

#include "Motion.glslh"

layout (location = 0) in vec3 a_Pos;
layout (location = 5) in vec4 a_Weights;
layout (location = 6) in ivec4 a_BoneIds;

void main() {
    gl_Position = GetPosition(a_Pos, a_BoneIds, a_Weights);
}

// Geometry stage
#version 430 core
#pragma stage : geo
precision mediump float;

layout (triangles) in;
// layout (triangle_strip, max_vertices=180) out;
// max_vertices is 3 * 6 * POINT_LIT_LIMIT
// 72 is value for for some hardware compatibility
// @todo: how to set max_vertices dynamically?
layout (triangle_strip, max_vertices=72) out;

#include "lighting/PointLitCaster.glslh"

// v_FragPos from geometry shader (output per emitvertex())
out g2f {
    vec4 v_FragPos;
    flat vec3 v_LitPos;
    flat float v_FrustumFar;
}; 

void main() {
    for(int i = 0; i < u_PointLitCaster.LitNum; ++i)
    {
        if(!u_PointLitCaster.LitAttr[i].CastShadow)
            continue;

        for(int face = 0; face < POINT_LIT_DIRECTION_NUM; ++face)
        {
            // Built-in variable that specifies to which face we render mainly for cube texture
            gl_Layer = i * POINT_LIT_DIRECTION_NUM + face; 
            for(int vertexIdx = 0; vertexIdx < 3; ++vertexIdx) // For each triangle's vertices
            {
                v_FragPos = gl_in[vertexIdx].gl_Position;
                v_LitPos = u_PointLitCaster.LitAttr[i].Position;
                v_FrustumFar = u_PointLitCaster.FrustumFar;
                gl_Position = u_PointLitCaster.LitAttr[i].LitSpaceMat[face] * v_FragPos;
                EmitVertex();
            }    
            EndPrimitive();
        }
    }    
}


// Fragment stage
#version 430 core
#pragma stage : frag
precision mediump float;

in g2f {
    vec4 v_FragPos;
    flat vec3 v_LitPos;
    flat float v_FrustumFar;
};

void main() {
    float lightDist = length(v_FragPos.xyz - v_LitPos);
    // Map to [0;1] range by dividing by far_plane
    lightDist = lightDist / v_FrustumFar;
    // Write this as modified depth
    gl_FragDepth = lightDist;
}