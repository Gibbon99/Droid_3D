#version 330

uniform mat4 u_viewProjectionMat;
uniform mat4 u_modelMat;
uniform mat3 u_normalMatrix;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTextureCoords;
layout(location = 2) in vec3 inNormal;

out vec3 WorldPos0;
out vec2 TexCoord0;
out vec3 Normal0;

mat4 NormalMatrix = transpose(inverse(u_modelMat));

void main()
{
    WorldPos0      = (u_modelMat * vec4(inPosition, 1.0)).xyz;

    Normal0        = normalize(u_normalMatrix * inNormal);

    TexCoord0      = inTextureCoords.xy;
	
    gl_Position    = u_viewProjectionMat * u_modelMat * vec4(inPosition, 1.0);
}
