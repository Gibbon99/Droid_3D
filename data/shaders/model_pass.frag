#version 330

in vec3 WorldPos0;
in vec3 Normal0;
in vec2 TexCoord0;
in vec3 uLightColor;

out vec3 WorldPosOut;
out vec3 NormalOut;
out vec4 DiffuseOut;

uniform sampler2D inTexture0;

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

//	vec3 tempLight = uLightColor * 10;

	DiffuseOut      = texture2D(inTexture0, TexCoord0) * vec4(uLightColor.rgb, 0.5f);

//	DiffuseOut.rgb = uLightColor.rgb;
}