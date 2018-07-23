#version 330

uniform sampler2D   inTexture0;
in      vec2        texCoord0;

out     vec4        outColor;

void main(void)
{
    float z = texture2D(inTexture0, texCoord0).r;       // fetch the z-value from our depth texture
    float n = 0.01;                                      // the near plane
    float f = 1000.0;                                   // the far plane
    float col = (2.0 * n) / (f + n - z * (f - n));      // convert to linear values

    outColor = vec4(col, col, col, 1.0);
}
