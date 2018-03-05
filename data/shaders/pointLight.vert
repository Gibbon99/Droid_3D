#version 330

layout(location = 0) in vec3 inPosition;

out vec4 fsPos;

uniform mat4 u_viewProjectionMat;
uniform mat4 u_modelMat;

uniform float 	uLightRadius;
uniform vec3 	uLightPosition;

void main()
{
	vec4 pos = u_viewProjectionMat  * vec4((inPosition * uLightRadius) + uLightPosition, 1.0);

	gl_Position = pos;

	fsPos = pos;
}