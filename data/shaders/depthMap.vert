#version 130

uniform mat4 u_viewProjectionMat;
uniform mat4 u_modelMat;

in vec3 inPosition;
in vec2 inTextureCoords;

out vec2 texCoord0;

void main(void)
{
	texCoord0 = inTextureCoords;

    gl_Position = u_viewProjectionMat * vec4(inPosition, 1.0);
}
