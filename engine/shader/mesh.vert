#version 430 core
precision mediump float;

const int  MAX_BONE_INFLUENCE = 4;
const int  MAX_BONES = 100;
const uint NULL_INDEX = 0xffffffffU;

layout (location = 0) in vec3  a_pos;
layout (location = 1) in vec3  a_normal;
layout (location = 2) in vec3  a_tangent;
layout (location = 3) in vec3  a_binormal;
layout (location = 4) in vec2  a_texcoord;
layout (location = 5) in vec4  a_weights;
layout (location = 6) in uvec4 a_boneIds; 

layout (std140) uniform Matrices
{
	mat4 ViewMat;
	mat4 PrjMat;
	mat4 Ortho;
    vec3 CameraPosition;
    float FrustumFar;
} u_matrices;

layout (std140) uniform BoneMatrices
{
	mat4 Transform[MAX_BONES];
} u_bones;

out VsOut {
    vec3 v_worldPos;
    vec3 v_normal;
    vec2 v_texCoord;
    flat mat3 v_TBN;
}; 

uniform bool u_playAnimation;
uniform bool u_useNormalMap;
uniform mat4 u_modelMat;

uniform samplerBuffer u_gpuAnimBuffer;
uniform bool u_bGpuAnimated;
uniform int  u_frameIndex;
uniform int  u_boneNum;

mat4 gpuAnimated(int boneId);
mat4 skinning();
mat3 calcTBN(mat4 skinTransform);

void main() {
    mat4 skinTransform = skinning();
    vec4 pos = skinTransform * vec4(a_pos, 1.f);

	v_worldPos = vec3(u_modelMat * pos);
    v_normal = a_normal;
    v_texCoord = a_texcoord;
    v_TBN = calcTBN(skinTransform);

	gl_Position = u_matrices.PrjMat * u_matrices.ViewMat * u_modelMat * pos;
}

mat4 skinning()
{
    mat4 skinTransform = mat4(0.f);
    if(u_playAnimation)
    {
        for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
        {
            if(a_boneIds[i] == NULL_INDEX) 
                continue;           
            skinTransform += a_weights[i] * (u_bGpuAnimated ? gpuAnimated(int(a_boneIds[i])) : u_bones.Transform[a_boneIds[i]]);
        }
    }
    else
    {
        skinTransform = mat4(1.f);
    }
    return skinTransform;
}

mat3 calcTBN(mat4 skinTransform)
{
    mat3 TBN;
    if(u_useNormalMap)
    {
        vec3 T = normalize(vec3(u_modelMat * skinTransform * vec4(a_tangent, 0.f)));
        vec3 N = normalize(vec3(u_modelMat * skinTransform * vec4(a_normal, 0.f)));
        T = normalize(T - dot(T, N) * N);
        vec3 B = cross(N, T);
        TBN = mat3(T, B, N);
    }
    else
    {
        TBN = transpose(inverse(mat3(u_modelMat * skinTransform)));
    }
    return TBN;
}

mat4 gpuAnimated(int boneId)
{
    // bone mat(3x4)
    int start = 4 * u_boneNum * u_frameIndex + 4 * boneId;
    vec4 col0 = texelFetch(u_gpuAnimBuffer, start);
    vec4 col1 = texelFetch(u_gpuAnimBuffer, start + 1);
    vec4 col2 = texelFetch(u_gpuAnimBuffer, start + 2);
    vec4 col3 = texelFetch(u_gpuAnimBuffer, start + 3);

    mat4 boneMat;
    boneMat[0].x = col0.x;
    boneMat[0].y = col0.y;
    boneMat[0].z = col0.z;
    boneMat[0].w = 0.f;

    boneMat[1].x = col1.x;
    boneMat[1].y = col1.y;
    boneMat[1].z = col1.z;
    boneMat[1].w = 0.f;

    boneMat[2].x = col2.x;
    boneMat[2].y = col2.y;
    boneMat[2].z = col2.z;
    boneMat[2].w = 0.f;

    boneMat[3].x = col3.x;
    boneMat[3].y = col3.y;
    boneMat[3].z = col3.z;
    boneMat[3].w = 1.f;

    return boneMat;
}