#version 330 core
in vec3 inPosition;
in vec3 inNormal;
in vec2 inTextureCoords;
in vec2 inTextureCoords1;

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

//uniform bool reverse_normals;

void main()
{
    vs_out.FragPos = vec3(model * vec4(inPosition, 1.0));
//    if(reverse_normals) // a slight hack to make sure the outer large cube displays lighting from the 'inside' instead of the default 'outside'.
//        vs_out.Normal = transpose(inverse(mat3(model))) * (-1.0 * aNormal);
//    else
    vs_out.Normal = transpose(inverse(mat3(model))) * inNormal;
    vs_out.TexCoords = inTextureCoords;
    gl_Position = projection * view * model * vec4(inPosition, 1.0);
}