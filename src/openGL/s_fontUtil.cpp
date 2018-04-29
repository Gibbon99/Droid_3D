#include "s_globals.h"
#include "s_fontUtil.h"
#include "s_shaders.h"
#include "s_openGLWrap.h"

#include "vec234.h"
#include "vector.h"
#include "vera-16.h"
#include "utf8-utils.h"

const char *vertex_src = "#version 330\n"
                         "\n"
                         "in vec3 inPosition;\n"
                         "in vec2 inTextureCoords;\n"
                         "in vec4 fontColor;\n"
                         "\n"
                         "out vec4 theFontColor;\n"
                         "out vec2 texCoord0;\n"
                         "\n"
                         "void main(void)\n"
                         "{\n"
                         "    texCoord0 = inTextureCoords;\n"
                         "\n"
                         "    theFontColor = fontColor;\n"
                         "\n"
                         "    vec2 vertexPosition_homoneneousspace = inPosition.xy - vec2(400,300); // [0..800][0..600] -> [-400..400][-300..300]\n"
                         "    vertexPosition_homoneneousspace /= vec2(400,300);\n"
                         "    gl_Position =  vec4(vertexPosition_homoneneousspace,0,1);\n"
                         "}\n"
                         "";

const char *fragment_src=""
                       "#version 330\n"
                       "uniform sampler2D   inTexture0;\n"
                       "in      vec2        texCoord0;\n"
                       "in      vec4        theFontColor;\n"
                       "out     vec4        outColor;\n"
                       "\n"
                       "void main(void)\n"
                       "{\n"
                       "	float alpha = texture2D(inTexture0, texCoord0).r;\n"
                       "	outColor = vec4(theFontColor.rgb, theFontColor.a) * alpha;\n"
                       "}\n"
                       "";

typedef struct
{
	glm::vec2		screenPosition;
} _fnt_glyph;

_fnt_glyph			fnt_glyph;

typedef struct
{
	glm::vec4		P0;
	glm::vec4		P1;
	glm::vec4		P2;

	glm::vec4		P3;
	glm::vec4		P4;
	glm::vec4		P5;
} _fnt_colorBuffer;

typedef struct
{
	glm::vec3		P0;		// Triangle One
	glm::vec3		P1;
	glm::vec3		P2;

	glm::vec3		P3;		// Triangle Two
	glm::vec3		P4;
	glm::vec3		P5;

} _fnt_vertBuffer;

typedef struct
{
	glm::vec2		P0;
	glm::vec2		P1;
	glm::vec2		P2;

	glm::vec2		P3;
	glm::vec2		P4;
	glm::vec2		P5;
} _fnt_textureBuffer;

GLuint 				embeddedTexID;

GLuint				fnt_g_glyphVAO_ID;
GLuint				fnt_g_vertVBO_ID;
GLuint				fnt_g_texVBO_ID;
GLuint				fnt_g_colVBO_ID;

GLuint				fnt_vertexShader_ID;
GLuint				fnt_fragmentShader_ID;
GLuint				fnt_shaderProgram_ID;

GLuint				fnt_inPosition_ID;
GLuint				fnt_inTextureCoords_ID;
GLuint				fnt_inColor_ID;
GLuint				fnt_inTextureUnit_ID;

vector<_fnt_vertBuffer>			fnt_vertBuffer;
vector<_fnt_textureBuffer>		fnt_textureBuffer;
vector<_fnt_colorBuffer>		fnt_colorBuffer;


// -------------------------------------------------------------------
//
// Show info from compiling shaders
void fnt_printGLInfoLog (const char *log_title, GLuint handle, const char *shader_name = 0)
// -------------------------------------------------------------------
{
	GLchar *glString;
	GLint size;

	size = 0;
	glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &size);

	if (size > 1)
		{
			glString = new GLchar [size];
			glGetShaderInfoLog(handle, size, &size, glString);
			con_print(CON_INFO, true, "%s info log", log_title);

			if(shader_name != 0)
				con_print(CON_INFO, true, " for %s:\n", shader_name);

			printf("-----------\n%s\n-----------\n", glString);
			delete [] glString;
		}
}

// -------------------------------------------------------------------
//
// Compile and link the shaders for the embedded font
// Text is in source so no external files are needed
bool fnt_compileLinkShaders()
// -------------------------------------------------------------------
{
	GLint		linked;
	GLint		len;
	
	fnt_vertexShader_ID = glCreateShader(GL_VERTEX_SHADER);
	len = (GLint)strlen(vertex_src);
	glShaderSource(fnt_vertexShader_ID, 1, &vertex_src, &len);
	glCompileShader(fnt_vertexShader_ID);
	fnt_printGLInfoLog("Compile", fnt_vertexShader_ID, "Embedded Font Vertex Shader");

	fnt_fragmentShader_ID = glCreateShader(GL_FRAGMENT_SHADER);
	len = (GLint)strlen(fragment_src);
	glShaderSource(fnt_fragmentShader_ID, 1, &fragment_src, &len);
	glCompileShader(fnt_fragmentShader_ID);
	fnt_printGLInfoLog("Compile", fnt_fragmentShader_ID, "Embedded Font Fragment Shader");

	fnt_shaderProgram_ID = glCreateProgram();
	glAttachShader(fnt_shaderProgram_ID, fnt_vertexShader_ID);
	glAttachShader(fnt_shaderProgram_ID, fnt_fragmentShader_ID);

	glLinkProgram(fnt_shaderProgram_ID);
	
	glGetProgramiv ( fnt_shaderProgram_ID, GL_LINK_STATUS, &linked );

	if ( true == linked )
		{
			con_print ( CON_TEXT, true, "INFO: Shaders linked ok - [ %s ]", "Embedded font Shader." );
			
			glDetachShader(fnt_shaderProgram_ID, fnt_vertexShader_ID);
			glDeleteShader(fnt_vertexShader_ID);

			glDetachShader(fnt_shaderProgram_ID, fnt_fragmentShader_ID);
			glDeleteShader(fnt_fragmentShader_ID);
			
			glUseProgram(fnt_shaderProgram_ID);
			
			fnt_inPosition_ID = glGetAttribLocation(fnt_shaderProgram_ID, 		"inPosition");
			fnt_inTextureCoords_ID = glGetAttribLocation(fnt_shaderProgram_ID, "inTextureCoords");
			fnt_inColor_ID = glGetAttribLocation(fnt_shaderProgram_ID, 		"fontColor");
			fnt_inTextureUnit_ID = glGetUniformLocation(fnt_shaderProgram_ID,  	"inTexture0");
			return true;
		}

	else
		{
			con_print ( CON_TEXT, true, "ERROR: Shaders failed to link - [ %s ]", "Embedded font Shader." );
			return false;
		}
}

// -------------------------------------------------------------------
//
// Create texture containing the embedded font data
// Construct the texture and vertex positions and upload to card
// Render buffers using GL_TRIANGLES
void fnt_printText(glm::vec2 position, glm::vec4 lineColor, const char *text, ... )
// -------------------------------------------------------------------
{
#define POS_Z	0.0

	static bool					initDone = false;
	size_t						i, j;
	float						currentX, currentY;
	_fnt_vertBuffer				fnt_vertBufferTemp;
	_fnt_textureBuffer			fnt_textureBufferTemp;
	_fnt_colorBuffer			fnt_colorBufferTemp;

	va_list						args;
	char						textLine[MAX_STRING_SIZE];
	//
	// get out the passed in parameters
	//
	va_start ( args, text );
	vsnprintf ( textLine, MAX_STRING_SIZE, text, args );
	va_end ( args );

	currentX = position.x;
	currentY = position.y;

	fnt_vertBuffer.clear();
	fnt_textureBuffer.clear();
	fnt_colorBuffer.clear();

	if (initDone == false)
		{
			glGenTextures( 1, &embeddedTexID );
			glBindTexture( GL_TEXTURE_2D, embeddedTexID );

			glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

			glTexImage2D( GL_TEXTURE_2D, 0, GL_R8, embeddedFontData.tex_width, embeddedFontData.tex_height, 0, GL_RED, GL_UNSIGNED_BYTE, embeddedFontData.tex_data );
			//
			// Setup the Vertex Array Object that will have the VBO's associated to it
			GL_ASSERT ( glGenVertexArrays ( 1, &fnt_g_glyphVAO_ID ) );
			//
			// Bind the vertex info
			GL_ASSERT ( glGenBuffers ( 1, &fnt_g_vertVBO_ID ) );
			GL_ASSERT ( glGenBuffers ( 1, &fnt_g_texVBO_ID ) );
			GL_ASSERT ( glGenBuffers ( 1, &fnt_g_colVBO_ID ) );

			if (false == fnt_compileLinkShaders())
				return;
				
			initDone = true;
		}

	//
	// Populate the vectors with the vertex and texture information for this line of text
	for (i = 0; i != strlen(textLine); i++)
		{
			texture_glyph_t		*glyph;
			float				offset_y;

			for (j = 0; j < embeddedFontData.glyphs_count; ++j)
				{

					if (embeddedFontData.glyphs[j].codepoint == ftgl::utf8_to_utf32 (textLine + i))
						{
							glyph = &embeddedFontData.glyphs[j];
							offset_y = glyph->height - glyph->offset_y;
							break;
						}
				}

			if (!glyph)
				continue;

			fnt_textureBufferTemp.P0.x = glyph->s0;		// 1st Triangle vertex 0
			fnt_textureBufferTemp.P0.y = glyph->t1;
			fnt_vertBufferTemp.P0.x = currentX + glyph->offset_x;
			fnt_vertBufferTemp.P0.y = currentY - offset_y;
			fnt_vertBufferTemp.P0.z = POS_Z;
			fnt_colorBufferTemp.P0.r = lineColor.r;
			fnt_colorBufferTemp.P0.g = lineColor.g;
			fnt_colorBufferTemp.P0.b = lineColor.b;
			fnt_colorBufferTemp.P0.a = lineColor.a;

			fnt_textureBufferTemp.P1.x = glyph->s0;
			fnt_textureBufferTemp.P1.y = glyph->t0;
			fnt_vertBufferTemp.P1.x = currentX + glyph->offset_x;
			fnt_vertBufferTemp.P1.y = (currentY + glyph->height) - offset_y;
			fnt_vertBufferTemp.P1.z = POS_Z;
			fnt_colorBufferTemp.P1.r = lineColor.r;
			fnt_colorBufferTemp.P1.g = lineColor.g;
			fnt_colorBufferTemp.P1.b = lineColor.b;
			fnt_colorBufferTemp.P1.a = lineColor.a;

			fnt_textureBufferTemp.P2.x = glyph->s1;
			fnt_textureBufferTemp.P2.y = glyph->t0;
			fnt_vertBufferTemp.P2.x = currentX + glyph->width + glyph->offset_x;
			fnt_vertBufferTemp.P2.y = (currentY + glyph->height) - offset_y;
			fnt_vertBufferTemp.P2.z = POS_Z;
			fnt_colorBufferTemp.P2.r = lineColor.r;
			fnt_colorBufferTemp.P2.g = lineColor.g;
			fnt_colorBufferTemp.P2.b = lineColor.b;
			fnt_colorBufferTemp.P2.a = lineColor.a;


			fnt_textureBufferTemp.P3.x = glyph->s0;		// 2nd Triangle vertex 0
			fnt_textureBufferTemp.P3.y = glyph->t1;
			fnt_vertBufferTemp.P3.x = currentX + glyph->offset_x;
			fnt_vertBufferTemp.P3.y = currentY - offset_y;
			fnt_vertBufferTemp.P3.z = POS_Z;
			fnt_colorBufferTemp.P3.r = lineColor.r;
			fnt_colorBufferTemp.P3.g = lineColor.g;
			fnt_colorBufferTemp.P3.b = lineColor.b;
			fnt_colorBufferTemp.P3.a = lineColor.a;

			fnt_textureBufferTemp.P4.x = glyph->s1;
			fnt_textureBufferTemp.P4.y = glyph->t0;
			fnt_vertBufferTemp.P4.x = currentX + glyph->width + glyph->offset_x;
			fnt_vertBufferTemp.P4.y = (currentY + glyph->height) - offset_y;
			fnt_vertBufferTemp.P4.z = POS_Z;
			fnt_colorBufferTemp.P4.r = lineColor.r;
			fnt_colorBufferTemp.P4.g = lineColor.g;
			fnt_colorBufferTemp.P4.b = lineColor.b;
			fnt_colorBufferTemp.P4.a = lineColor.a;

			fnt_textureBufferTemp.P5.x = glyph->s1;
			fnt_textureBufferTemp.P5.y = glyph->t1;
			fnt_vertBufferTemp.P5.x = currentX + glyph->width + glyph->offset_x;
			fnt_vertBufferTemp.P5.y = currentY - offset_y;
			fnt_vertBufferTemp.P5.z = POS_Z;
			fnt_colorBufferTemp.P5.r = lineColor.r;
			fnt_colorBufferTemp.P5.g = lineColor.g;
			fnt_colorBufferTemp.P5.b = lineColor.b;
			fnt_colorBufferTemp.P5.a = lineColor.a;

			currentX += glyph->advance_x;
			currentY += glyph->advance_y;

			fnt_textureBuffer.push_back(fnt_textureBufferTemp);
			fnt_vertBuffer.push_back(fnt_vertBufferTemp);
			fnt_colorBuffer.push_back(fnt_colorBufferTemp);
		}

	//
	// Now upload and draw the line of text
	//

	//
	// Start using TTF font shader program
	GL_CHECK ( glUseProgram ( fnt_shaderProgram_ID ) );
	//
	// Bind the generated VAO
	GL_CHECK ( glBindVertexArray ( fnt_g_glyphVAO_ID ) );

	wrapglEnable ( GL_BLEND );

	glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	wrapglDisable ( GL_DEPTH_TEST );
	//
	// Bind texture if it's not already bound as current texture
	wrapglBindTexture ( GL_TEXTURE0, embeddedTexID );
	GL_CHECK ( glUniform1i ( fnt_inTextureUnit_ID, 0 ) );
	//
	// Bind the vertex info
	GL_CHECK ( glBindBuffer ( GL_ARRAY_BUFFER, fnt_g_vertVBO_ID ) );
	GL_CHECK ( glBufferData ( GL_ARRAY_BUFFER, sizeof ( glm::vec3 ) * 6 * strlen(textLine), &fnt_vertBuffer[0], GL_DYNAMIC_DRAW ) );
	GL_CHECK ( glVertexAttribPointer ( fnt_inPosition_ID, 3, GL_FLOAT, false, 0, BUFFER_OFFSET ( 0 ) ) );
	GL_CHECK ( glEnableVertexAttribArray ( fnt_inPosition_ID ) );
	//
	// Bind the texture coordinates
	GL_CHECK ( glBindBuffer ( GL_ARRAY_BUFFER, fnt_g_texVBO_ID ) );
	GL_CHECK ( glBufferData ( GL_ARRAY_BUFFER, sizeof ( _fnt_textureBuffer ) * 6 * strlen(textLine), &fnt_textureBuffer[0], GL_DYNAMIC_DRAW ) );
	GL_CHECK ( glVertexAttribPointer ( fnt_inTextureCoords_ID, 2, GL_FLOAT, false, 0, BUFFER_OFFSET ( 0 ) ) );
	GL_CHECK ( glEnableVertexAttribArray ( fnt_inTextureCoords_ID ) );
	//
	// Bind color array
	GL_CHECK ( glBindBuffer ( GL_ARRAY_BUFFER, fnt_g_colVBO_ID ) );
	GL_CHECK ( glBufferData ( GL_ARRAY_BUFFER, sizeof ( _fnt_colorBuffer ) * 6 * strlen(textLine), &fnt_colorBuffer[0], GL_DYNAMIC_DRAW ) );
	GL_CHECK ( glVertexAttribPointer ( fnt_inColor_ID, 4, GL_FLOAT, false, 0, BUFFER_OFFSET ( 0 ) ) );
	GL_CHECK ( glEnableVertexAttribArray ( fnt_inColor_ID ) );
	//
	// No matrices used - transform is done in the shader
	// TODO: Pass in screen size dimensions to the shader to work out the proper screen size
	GL_CHECK ( glDrawArrays(GL_TRIANGLES, 0, sizeof ( glm::vec3 ) * 4 * strlen(textLine)));	// 4 should be 6 ??
}
