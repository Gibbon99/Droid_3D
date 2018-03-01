#version 330

in vec3 WorldPos0;
in vec3 Normal0;
in vec2 TexCoord0;

out vec4 DiffuseOut;
out vec3 WorldPosOut;
out vec4 NormalOut;

out vec4 fragColor;

uniform sampler2D inTexture0;

void main()
{
    //
    // Diffuse - texture image
	DiffuseOut      = texture(inTexture0, TexCoord0).rgba;
	
	DiffuseOut.rgba = vec4(200.5f, 0.0f, 0.0f, 0.5f);

    //
    // Position within the world
	WorldPosOut     = WorldPos0.xyz;
    //
    // Normals for the model
    NormalOut       = vec4(Normal0, 0);
//    NormalOut.xyz       = (Normal0 + vec3(0.0)) * 0.5;
//	NormalOut.w = 0.0;
//	TexCoordOut     = vec3(TexCoord0, 0.0);
}


 // w-component: 1 for positions and 0 for vectors

