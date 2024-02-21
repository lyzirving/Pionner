#version 330 core

precision highp float;

const int POINT_LIGHT_DIRECTION_NUM = 6;
const int POINT_LIGHT_LIMIT = 10;

layout (triangles) in;
// max_vertices is 3 * 6 * POINT_LIGHT_LIMIT
// layout (triangle_strip, max_vertices=180) out;

// max_vertices is 3 * 6 * 4(for hardware compatibility)
// TODO: how to set max_vertices dynamically? 
layout (triangle_strip, max_vertices=72) out; 

struct LightMetaData
{
    mat4 LightSpaceMat[POINT_LIGHT_DIRECTION_NUM];
    vec3 Position;
    float FrustumFar;
};

layout(std140) uniform PointLightShadowData
{
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
