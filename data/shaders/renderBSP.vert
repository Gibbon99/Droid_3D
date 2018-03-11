#version 330

uniform mat4 u_viewProjectionMat;
uniform mat4 u_modelMat;
uniform mat4 u_normalMatrix;

in vec3 inPosition;
in vec2 inTextureCoords;
in vec2 inTextureCoords1;
in vec3 inNormal;

out vec3 fragVert;
out vec2 fragTexCoord;
out vec2 fragLightmapCoord;
out vec3 fragNormal;

void main()
{
//    mat3 normalMatrix 	= transpose(inverse(mat3(u_modelMat)));
    fragNormal 			= normalize(inNormal * mat3(u_normalMatrix));

    // Pass some variables to the fragment shader
    fragTexCoord 		= inTextureCoords;
    fragVert 			= inPosition;
    fragLightmapCoord 	= inTextureCoords1;

//    gl_Position 		= u_viewProjectionMat * u_modelMat * vec4(inPosition, 1.0);
	
	gl_Position			= vec4(inPosition, 1.0);
}

