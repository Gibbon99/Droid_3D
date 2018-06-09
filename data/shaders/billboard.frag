#version 330

uniform sampler2D   inTexture0;

in float alphaValue_frag;

in  vec2    TexCoord0;
out vec4    FragColor;

void main()
{
	FragColor = texture2D(inTexture0, TexCoord0);

	FragColor.a = alphaValue_frag;

//	if (FragColor.r == 0 && FragColor.g == 0 && FragColor.b == 0) {
//		discard;
//	}
}