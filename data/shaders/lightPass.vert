#version 330

uniform mat4 u_viewProjectionMat;
uniform mat4 u_modelMat;

in 	vec3 inPosition;
in  vec2 inTextureCoords;

out vec2 fragTexCoord0;

void main(void)
{
    fragTexCoord0 = inTextureCoords;

    gl_Position = u_viewProjectionMat * u_modelMat * vec4(inPosition, 1.0);

    // screen space position of the vertex from the light source's point of view
    // plus it is mapped from range [-1, 1]
    // to [0, 1] by shadowMat in order to be used
    // as texture coordinates
}
