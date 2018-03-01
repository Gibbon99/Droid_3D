#version 330

in vec3 outPosition;
in vec3 outNormal;
in vec3 outTextureCoords;

in vec3 tPosition;
in vec3 tNormals;
in vec3 tDiffuse; 

uniform vec3 cameraPosition;

out vec4 geoData[3];

void main( void )
{

	geoData[0] = vec4(outPosition, 1);
	geoData[1] = vec4(outNormal, 1);
	geoData[2] = vec4(outTextureCoords, 1);
}