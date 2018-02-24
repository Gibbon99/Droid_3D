#version 330

uniform mat4 u_viewProjectionMat;
uniform mat4 u_modelMat;

in 	vec3 inPosition;

in  vec4 debugColor;
out vec4 outColor;

void main(void)
{
	outColor = debugColor;
//    gl_Position = mvp * vec4(inPosition, 1.0);
    gl_Position = u_viewProjectionMat * u_modelMat * vec4(inPosition, 1.0);
}

