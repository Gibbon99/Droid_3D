#version 330

//
// Need to write to the diffuse FBO texture to see the lines
//
in 	vec4	outColor;
out vec4	fragColor;

in vec3 WorldPos0;
in vec2 TexCoord0;

layout (location=0) out vec3 WorldPosOut;
layout (location=2) out vec3 DiffuseOut;

uniform sampler2D inTexture0;

void main(void)
{
    //
    // Position within the world
	WorldPosOut     = WorldPos0;
    //
    // Diffuse - texture image
	DiffuseOut      = texture2D(inTexture0, TexCoord0).xyz * outColor.rgb;
}

