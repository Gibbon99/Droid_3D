
/* Freetype GL - A C OpenGL Freetype engine
 *
 * Distributed under the OSI-approved BSD 2-Clause License.  See accompanying
 * file `LICENSE` for more details.
 */
#version 330

uniform vec2 inScreenSize;

in vec3 inPosition;
in vec2 inTextureCoords;
in vec4 fontColor;

out vec4 theFontColor;
out vec2 texCoord0;

void main(void)
{
	texCoord0 = inTextureCoords;
    theFontColor = fontColor;
	
	vec2 vertexPosition = inPosition.xy - inScreenSize.xy;
	vertexPosition /= inScreenSize.xy;
	gl_Position =  vec4(vertexPosition,0,1);
}