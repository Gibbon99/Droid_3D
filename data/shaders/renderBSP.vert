#version 330

uniform mat4 u_viewProjectionMat;
uniform mat4 u_modelMat;

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
    mat3 normalMatrix = transpose(inverse(mat3(u_modelMat)));

    // Pass some variables to the fragment shader
    fragTexCoord = inTextureCoords;
    fragNormal = inNormal; // * normalMatrix; //gl_NormalMatrix * vec4(inNormal, 1.0);
    fragVert = inPosition;
    fragLightmapCoord = inTextureCoords1;

//    Normal0        = normalize(u_normalMatrix * inNormal);

//    TexCoord0      = inTextureCoords;
    gl_Position    = u_viewProjectionMat * u_modelMat * vec4(inPosition, 1.0);
}

