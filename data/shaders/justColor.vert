#version 330

uniform mat4 u_viewProjectionMat;
uniform mat4 u_modelMat;

in vec3 inPosition;

out vec3 WorldPos0;

void main(void)
{
    // screen space position of the vertex from camera's point of view
    gl_Position    = u_viewProjectionMat * u_modelMat * vec4(inPosition, 1.0);

    WorldPos0      = gl_Position.xyz; //(u_modelMat * vec4(inPosition, 1.0)).xyz;
}
