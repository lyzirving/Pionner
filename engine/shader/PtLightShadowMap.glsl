/*// -- Pionner Engine Point Light Shadow Map shader --
// -------------------------------------------
// Filling the shadow map for point light
*/

// Vertex stage
#version 430 core
#pragma stage : vert
precision mediump float;

#include <Skeleton.glslh>

layout (location = 0) in vec3  a_pos;
layout (location = 1) in vec3  a_normal;
layout (location = 2) in vec3  a_tangent;
layout (location = 3) in vec3  a_binormal;
layout (location = 4) in vec2  a_texcoord;
layout (location = 5) in vec4  a_weights;
layout (location = 6) in uvec4 a_boneIds;

uniform bool u_playAnimation;
uniform bool u_bGpuAnimated;
uniform int u_frameIndex;
uniform int u_boneNum;
uniform samplerBuffer u_gpuAnimBuffer;

uniform mat4 u_modelMat;

void main() {
	mat4 boneTransform = CalcBoneTransform(u_playAnimation, u_bGpuAnimated, a_weights, a_boneIds, u_frameIndex, u_boneNum, u_gpuAnimBuffer);

	gl_Position = u_modelMat * boneTransform * vec4(a_pos, 1.f);
}

// Geometry stage
#version 430 core
#pragma stage : geo
precision mediump float;

const int POINT_LIGHT_DIRECTION_NUM = 6;
const int POINT_LIGHT_LIMIT = 4;

layout (triangles) in;

// layout (triangle_strip, max_vertices=180) out;
// max_vertices is 3 * 6 * POINT_LIGHT_LIMIT
// 72 is value for for some hardware compatibility
// TODO: how to set max_vertices dynamically?
layout (triangle_strip, max_vertices=72) out; 

struct LightMetaData
{
    mat4 LightSpaceMat[POINT_LIGHT_DIRECTION_NUM];
    vec3 Position;
    float FrustumFar;
    bool CastShadow;
};

layout(std140) uniform PointLightShadowData
{
    // TODO:????
    // The num of LightMeataData array will influence the fps
    // POINT_LIGHT_LIMIT used to be set 10, and the fps is slowed down
    // The slow down of fps is caused by the limit of bandwidth?
    int LightCount;
	LightMetaData LightData[POINT_LIGHT_LIMIT];
} u_shadowData;

// v_fragPos from geometry shader (output per emitvertex())
out vec4 v_fragPos;
flat out vec3 v_lightPos;
flat out float v_frustumFar;

void main() {
    for(int i = 0; i < u_shadowData.LightCount; i++)
    {
        if(!u_shadowData.LightData[i].CastShadow)
            continue;

        for(int face = 0; face < POINT_LIGHT_DIRECTION_NUM; ++face)
        {
            // Built-in variable that specifies to which face we render mainly for cube texture
            gl_Layer = i * POINT_LIGHT_DIRECTION_NUM + face; 
            for(int vertex = 0; vertex < 3; ++vertex) // For each triangle's vertices
            {
                v_fragPos = gl_in[vertex].gl_Position;
                v_lightPos = u_shadowData.LightData[i].Position;
                v_frustumFar = u_shadowData.LightData[i].FrustumFar;
                gl_Position = u_shadowData.LightData[i].LightSpaceMat[face] * v_fragPos;
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

in vec4 v_fragPos;
flat in vec3 v_lightPos;
flat in float v_frustumFar;

void main() {
    float lightDist = length(v_fragPos.xyz - v_lightPos);

    // Map to [0;1] range by dividing by far_plane
    lightDist = lightDist / v_frustumFar;

    // Write this as modified depth
    gl_FragDepth = lightDist;
}