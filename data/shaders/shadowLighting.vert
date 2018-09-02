#version 330 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTextureCoords;

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vs_out.FragPos = vec3(model * vec4(inPosition, 1.0));
     vs_out.Normal = transpose(inverse(mat3(model))) * inNormal;
  vs_out.TexCoords = inTextureCoords;
       gl_Position = projection * view * model * vec4(inPosition, 1.0);
}