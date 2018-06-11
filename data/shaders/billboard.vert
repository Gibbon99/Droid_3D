#version 330

layout (location = 0) in vec3 inPosition;
layout (location = 1) in float alphaValue_in;
layout (location = 2) in float sizeValue_in;

out float alphaValue_geom;
out float sizeValue_geom;

void main()
{
    gl_Position = vec4(inPosition, 1.0);
    alphaValue_geom = alphaValue_in;
    sizeValue_geom = sizeValue_in;
}