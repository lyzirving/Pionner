// specify the minimum version of opengl glsl, if current version is lower than this, the shader will not complie
#version 330 core

precision highp float;

const vec2  GRID_SQUARE = vec2(0.5f, 0.5f);
const float LINE_WIDTH = 0.03f;

uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_prjMat;

uniform vec4 u_lineColor;

in vec3 v_pos;

out vec4 o_color;

bool isXAxis(vec2 xzCoord);
bool isZAxis(vec2 xzCoord);
bool isGridLine(vec2 xzCoord);
float computeDepth(vec3 pos);

void main() {
    vec2 xzCoord;
    xzCoord.x = v_pos.x;
    xzCoord.y = v_pos.z;

    if(isXAxis(xzCoord))
    {
        o_color = vec4(1.f, 0.f, 0.f, 1.f);
    }
    else if(isZAxis(xzCoord))
    {
        o_color = vec4(0.f, 0.f, 1.f, 1.f);
    }
    else if(isGridLine(xzCoord))
    {
        o_color = u_lineColor;
    }
    else
    {
        o_color = vec4(0.f, 0.f, 0.f, 0.f);
    }
    gl_FragDepth = computeDepth(v_pos);
}

bool isXAxis(vec2 xzCoord)
{
    float zAbsVal = abs(xzCoord.y);
    return zAbsVal <= LINE_WIDTH;
}

bool isZAxis(vec2 xzCoord)
{
    float xAbsVal = abs(xzCoord.x);
    return xAbsVal <= LINE_WIDTH;
}

bool isGridLine(vec2 xzCoord)
{
    vec2 fractPart = fract(xzCoord / GRID_SQUARE);
    vec2 coordInOneGrid = fractPart * GRID_SQUARE;
    return abs(coordInOneGrid.x) <= LINE_WIDTH || abs(coordInOneGrid.y) <= LINE_WIDTH;
}

float computeDepth(vec3 pos)
{
    vec4 posClipSpace = u_prjMat * u_viewMat * u_modelMat * vec4(pos, 1.f);
    return (posClipSpace.z / posClipSpace.w);
}