#version 330

layout (location = 0) in vec3 inPosition;
layout (location = 1) in float alphaValue_in;

out float alphaValue_geom;

void main()
{
    gl_Position = vec4(inPosition, 1.0);
    alphaValue_geom = alphaValue_in;
}