#version 330

layout (location=0) in vec3 inPosition;
layout (location=1) in vec3 inNormal;
layout (location=2) in vec2 inTextureCoords;

uniform mat4 u_viewProjectionMat;
uniform mat4 u_modelMat;

out vec2 TexCoord0;
out vec3 Normal0;
out vec3 WorldPos0;

void main()
{
    vec4 worldPos   = u_modelMat * vec4(inPosition, 1.0);

    TexCoord0		= inTextureCoords;

	Normal0 		= vec4(inNormal, 0.0).xyz;

	WorldPos0      	= (u_modelMat * vec4(inPosition, 1.0)).xyz;
	
	gl_Position    	= u_viewProjectionMat * worldPos;
}
