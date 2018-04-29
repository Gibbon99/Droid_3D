#version 330
//
// Which texture to use
uniform sampler2D   inTexture0;
in      vec2        texCoord0;
//
// What color to draw the pixels
in      vec4        theFontColor;
out     vec4        outColor;

void main(void)
{
	float alpha = texture2D(inTexture0, texCoord0).r;
	outColor = vec4(theFontColor.rgb, theFontColor.a) * alpha;
//    outColor = vec4(1, 1, 1, texture2D(inTexture0, texCoord0).r) * theFontColor;
}

