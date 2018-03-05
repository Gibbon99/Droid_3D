#version 330

uniform mat4 u_viewProjectionMat;
uniform mat4 u_modelMat;

in vec2 inPosition;
in vec3 inTextureCoords;

out vec2 fragTexCoord0;

void main(void)
{
	fragTexCoord0 = inTextureCoords.xy;
    
    gl_Position    = u_modelMat * vec4(inPosition.xy, 0.0, 1.0); //u_viewProjectionMat * u_modelMat * vec4(inPosition, 1.0);
}

