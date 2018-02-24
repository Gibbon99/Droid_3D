#version 330

uniform mat4 u_viewProjectionMat;
uniform mat4 u_modelMat;
uniform mat4 u_shadowMat;

in 	vec3 inPosition;
in  vec2 inTextureCoords;
in  vec3 inNormal;

out vec3 fragVert;
out vec2 fragTexCoord;
out vec3 fragNormal;
out vec4 o_shadowCoord;

void main(void)
{
    fragTexCoord    = inTextureCoords;
    fragNormal      = inNormal;
    fragVert        = inPosition;

    // screen space position of the vertex from the light source's point of view
    // plus it is mapped from range [-1, 1]
    // to [0, 1] by shadowMat in order to be used
    // as texture coordinates
    o_shadowCoord = u_shadowMat * u_modelMat * vec4(inPosition, 1);;

    // screen space position of the vertex from camera's point of view
    gl_Position    = u_viewProjectionMat * u_modelMat * vec4(inPosition, 1.0);
}
