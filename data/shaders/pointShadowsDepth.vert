#version 330 core
in vec3 inPosition;

uniform mat4 u_modelMat;

void main()
{
    gl_Position = u_modelMat * vec4(inPosition, 1.0);
}