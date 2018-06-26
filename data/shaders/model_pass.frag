#version 330

in vec3 WorldPos0;
in vec3 Normal0;
in vec2 TexCoord0;
//in vec2 TexCoord1;

layout (location=0) out vec3 WorldPosOut;
layout (location=1) out vec3 NormalOut;
layout (location=2) out vec3 DiffuseOut;

uniform sampler2D inTexture0;
//uniform sampler2D inTexture1;

void main()
{
	//
	// Position within the world
	WorldPosOut     = WorldPos0;
	//
	// Normals for the model
	NormalOut       = normalize(Normal0);
	//
	// Diffuse - texture image
//	DiffuseOut.xyz      = texture2D(inTexture1, TexCoord1).xyz * texture2D(inTexture0, TexCoord0).xyz;
	DiffuseOut      = texture2D(inTexture0, TexCoord0).xyz;

}