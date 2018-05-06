/* Freetype GL - A C OpenGL Freetype engine
 *
 * Distributed under the OSI-approved BSD 2-Clause License.  See accompanying
 * file `LICENSE` for more details.
 */
#version 330
 
uniform sampler2D inTexture0;

in      vec2        texCoord0;
in      vec4        theFontColor;

out     vec4        outColor;

void main(void)
{
    float dist = texture2D(inTexture0, texCoord0.st).r;
    float width = fwidth(dist);
    float alpha = smoothstep(0.5-width, 0.5+width, dist);
    outColor = vec4(theFontColor.rgb, alpha * theFontColor.a);
}
