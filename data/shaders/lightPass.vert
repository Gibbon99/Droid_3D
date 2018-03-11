#version 330

in vec3 inPosition;
in vec3 inTextureCoords;

out vec2 fragTexCoord0;

void main(void)
{
	fragTexCoord0 = inTextureCoords.st;
    
	gl_Position = vec4(inPosition, 1.0);
}
