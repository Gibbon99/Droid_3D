#version 330

uniform mat4 u_viewProjectionMat;
uniform mat4 u_modelMat;

in vec3 inPosition;
in vec2 inTextureCoords;
in vec4 fontColor;

out vec4 theFontColor;
out vec2 texCoord0;

void main(void)
{
    texCoord0 = inTextureCoords;

    theFontColor = fontColor;

    gl_Position = u_viewProjectionMat * u_modelMat * vec4(inPosition, 1.0);
}
