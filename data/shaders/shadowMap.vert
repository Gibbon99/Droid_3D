#version 330

// matrices
uniform mat4 u_modelMat;
uniform mat4 u_viewProjectionMat;
uniform mat4 u_shadowMat;


// attributes
in 	vec3 inPosition;
in  vec3 inNormal;
in  vec2 inTextureCoords;


in float shininess;
in vec4 specular;
//uniform mat4 mv_matrix;
uniform mat4 v_matrix;

in vec3 lightPosition;

out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texcoord;
out float frag_shininess;
out vec4 frag_specular;
out vec3 lightPosition0;

out mat4 mv_matrix0;


// texture coordinates to the fragment shader
// for access to the shadow map
out vec4        o_shadowCoord;
out vec2        texCoord0;

void main(void)
{

    mat4 mv_matrix0 = v_matrix * u_modelMat;

    vec4 eye_position = mv_matrix0 * vec4(inPosition, 1.0);
//    gl_Position = p_matrix * eye_position;
    frag_position = eye_position.xyz;
    frag_normal   = (mv_matrix0 * vec4(inNormal, 0.0)).xyz;
    frag_texcoord = inTextureCoords;
    frag_shininess = shininess;
    frag_specular = specular;

    lightPosition0 = lightPosition;




    texCoord0 = inTextureCoords;
    //
    // position of the vertex in the scene
    vec4 worldPos = u_modelMat * vec4(inPosition, 1);

    // screen space position of the vertex from the light source's point of view
    // plus it is mapped from range [-1, 1]
    // to [0, 1] by shadowMat in order to be used
    // as texture coordinates
    o_shadowCoord = u_shadowMat * worldPos;

    // screen space position of the vertex from camera's point of view
    gl_Position = u_viewProjectionMat * u_modelMat * vec4(inPosition, 1.0);



}
