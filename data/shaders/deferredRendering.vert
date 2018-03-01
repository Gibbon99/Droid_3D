#version 330

uniform mat4 u_viewProjectionMat;
uniform mat4 u_modelMat;

in vec3 inPosition;
in vec3 inNormal;
in vec3 inTextureCoords;

out vec3 outPosition;
out vec3 outNormal;
out vec3 outTextureCoords;

void main( void )
{
	outPosition = inPosition;
	outNormal = inNormal;
	outTextureCoords = inTextureCoords;
	
//	gl_Position = u_viewProjectionMat * gl_Vertex;
//	gl_TexCoord[0] = gl_MultiTexCoord0;

//    gl_FrontColor = vec4(1.0, 1.0, 1.0, 1.0);
	
	gl_Position = u_viewProjectionMat * u_modelMat * vec4(inPosition, 1.0);
}