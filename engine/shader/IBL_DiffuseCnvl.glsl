/*// -- Pionner Engine IBL Diffuse Convolution Shader --
// ------------------------------------------------------------
*/
#version 430 core
#pragma stage : vert
precision mediump float;

layout (location = 0) in vec3 a_pos;

uniform mat4 u_viewMat;
uniform mat4 u_prjMat;

out vec3 v_localPos;

void main()
{
    v_localPos  = a_pos;
    gl_Position = u_prjMat * u_viewMat * vec4(a_pos, 1.0);
}

#version 430 core 
#pragma stage : frag 
precision mediump float;

#include <Common.glslh>

uniform samplerCube u_envMap;

in vec3  v_localPos;
out vec4 o_fragColor;

void main()
{		
	// the sample direction equals the hemisphere's orientation 
    vec3 N = normalize(v_localPos);
	vec3 irradiance = vec3(0.0);

	vec3 up = vec3(0.0, 1.0, 0.0);// world up
	vec3 right = cross(up, N);    // right of tangent space
	up = cross(N, right);         // up of tangent space

	float sampleDelta = 0.025;
	float nrSamples = 0.0; 
	for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
	{
		for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
		{
			// spherical to cartesian in tangent space
			vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
			// tangent space to world
			vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N; 

			irradiance += texture(u_envMap, sampleVec).rgb * cos(theta) * sin(theta);
			nrSamples++;
		}
	}
	irradiance = PI * irradiance * (1.0 / float(nrSamples));
	o_fragColor = vec4(irradiance, 1.0);
}