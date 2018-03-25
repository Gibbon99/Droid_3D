#version 330

uniform mat4 u_viewProjectionMat;
uniform mat4 u_modelMat;

layout (location=0) in vec3 inPosition;
layout (location=1) in vec3 inNormal;
layout (location=2) in vec2 inTextureCoords;

out vec2 TexCoord0;
out vec3 WorldPos0;

in  vec4 debugColor;
out vec4 outColor;

void main(void)
{
	outColor 		= debugColor;

	TexCoord0		= inTextureCoords;

	WorldPos0      	= (u_modelMat * vec4(inPosition, 1.0)).xyz;

    gl_Position 	= u_viewProjectionMat * u_modelMat * vec4(inPosition, 1.0);
}

