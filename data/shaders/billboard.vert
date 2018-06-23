#version 330

uniform sampler2D   inTexture0;

in vec3 inPosition;
in float alphaValue_in;
in float sizeValue_in;

out float alphaValue_geom;
out float sizeValue_geom;

void main()
{
    gl_Position = vec4(inPosition, 1.0);
    alphaValue_geom = alphaValue_in;
    sizeValue_geom = sizeValue_in;
}