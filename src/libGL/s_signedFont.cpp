#include "s_signedFont.h"
#include "s_shaders.h"
#include "s_openGLWrap.h"
#include "io_textures.h"
#include "io_fileSystem.h"

#define NUM_VERTS_PER_GLYPH 4

bool				fontInfoDone = false;
bool				fontInitDone = false;

struct _sdf_Vertex
{
	glm::vec2		position;
	glm::vec2		textureCoord;
	glm::vec4		color;
};

struct _font
{
	bool					isAvailable;
	bool					isLoaded;
	uint					size;
	uint					baseIndex;		// Starting index for indexed drawing - points to vertexes to make triangles
	float					currentX;
	float					currentY;
	GLuint					glyphVAO_ID;
	GLuint					index_ID;
	GLuint					vertexVBO_ID;
	string					fileName;
	texture_font_t			*font;
	texture_atlas_t			*atlas;
};

_font						font[MAX_NUM_FONTS];

vector<_sdf_Vertex>			fontVertex[MAX_NUM_FONTS];
vector<unsigned int>		vertIndex[MAX_NUM_FONTS];

const char * cache = " !\"#$%&'()*+,-./0123456789:;<=>?"
                     "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
                     "`abcdefghijklmnopqrstuvwxyz{|}~";

//------------------------------------------------------------------
//
// Add information for font
void sdf_addFontInfo(uint whichFont, string fileName, uint fontSize)
//------------------------------------------------------------------
{
	font[whichFont].fileName = fileName;
	font[whichFont].size = fontSize;
	font[whichFont].isLoaded = false;
	font[whichFont].baseIndex = 0;
	font[whichFont].currentX = 0.0f;
	font[whichFont].currentY = 0.0f;
	font[whichFont].isAvailable = io_doesFileExist(fileName);

	fontVertex[whichFont].clear();
	vertIndex[whichFont].clear();

	if (!font[whichFont].isAvailable)
		con_print(CON_ERROR, true, "Font [ %s ] is not available. Check fileName.", fileName.c_str());

	else
		{
			con_print(CON_INFO, true, "Font [ %s ] is available for use.", fileName.c_str());
			fontInfoDone = true;	// We have at least one font to use
		}
}

//------------------------------------------------------------------
//
// Init the font system
bool sdf_initFontSystem()
//------------------------------------------------------------------
{
	void*			filePointer;
	int				fontFileSize;

	if (false == fontInfoDone)
		{
			con_print(CON_ERROR, true, "Can not start font system without data. Run addFontInfo.");
			return false;
		}

	//
	// For each font
	for (int i = 0; i != MAX_NUM_FONTS; i++)
		{
			if (font[i].isAvailable)
				{
					font[i].atlas = texture_atlas_new( 512, 512, 1);

					fontFileSize = io_getFileSize((char *)font[i].fileName.c_str());

					if (-1 == fontFileSize)
						{
							con_print(CON_ERROR, true, "Unable to get file size for [ %s ]", font[i].fileName.c_str());
							break;
						}

					filePointer = ( GLint * ) malloc ( sizeof ( char ) * fontFileSize );

					if ( NULL == filePointer )
						{
							con_print(CON_ERROR, true, "Unable to get memory to hold file [ %s ]", font[i].fileName.c_str());
							break;
						}

					if ( -1 == io_getFileIntoMemory ( (char *)font[i].fileName.c_str(), (void *)filePointer ) )
						return false;


					//font[i].font = texture_font_new_from_file (font[i].atlas, font[i].size, font[i].fileName.c_str());
					font[i].font = texture_font_new_from_memory (font[i].atlas, font[i].size, filePointer, fontFileSize);

					if (NULL == font[i].font)
						{
							con_print(CON_INFO, true, "ERROR: Could not start SDF Fonts. Missing file [ %s ]", font[i].fileName.c_str());
							font[i].isAvailable = false;
							font[i].isLoaded = false;
						}

					else
						{
							font[i].font ->rendermode = RENDER_SIGNED_DISTANCE_FIELD;
							//
							// Populate the texture with the glyphs
							texture_font_load_glyphs( font[i].font, cache );

							glGenTextures( 1, &font[i].atlas->id );
							glBindTexture( GL_TEXTURE_2D, font[i].atlas->id );
							glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
							glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
							glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
							glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
							glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, font[i].atlas->width, font[i].atlas->height, 0, GL_RED, GL_UNSIGNED_BYTE, font[i].atlas->data );
							//
							// Setup the Vertex Array Object that will have the VBO's associated to it
							GL_ASSERT ( glGenVertexArrays ( 1, &font[i].glyphVAO_ID ) );

							GL_ASSERT ( glGenBuffers ( 1, &font[i].index_ID ) );

							GL_ASSERT ( glGenBuffers ( 1, &font[i].vertexVBO_ID) );
						}

					fontVertex[i].clear();
					vertIndex[i].clear();
				}
		}

	fontInitDone = true;
	return true;
}

//------------------------------------------------------------------
//
// Populate the temporary vertex and upload to array
void sdf_addVertexInfo(uint whichFont, glm::vec2 position, glm::vec2 textureCoord, glm::vec4 color)
//------------------------------------------------------------------
{
	_sdf_Vertex		sdf_vertexTemp;				// Temp to hold vertex info before adding to vector

	sdf_vertexTemp.position = position;
	sdf_vertexTemp.textureCoord = textureCoord;
	sdf_vertexTemp.color = color;

	fontVertex[whichFont].push_back(sdf_vertexTemp);
}

//------------------------------------------------------------------
//
// Add a string of text to the VBO
void sdf_addText(uint whichFont, glm::vec2 position, glm::vec4 lineColor, const char *text, ... )
//------------------------------------------------------------------
{
	va_list						args;
	char						textLine[MAX_STRING_SIZE];
	//
	// get out the passed in parameters
	//
	va_start ( args, text );
	vsnprintf ( textLine, MAX_STRING_SIZE, text, args );
	va_end ( args );

	if (!fontInitDone)
		{
			sdf_initFontSystem();
		}

	font[whichFont].currentX = position.x;
	font[whichFont].currentY = position.y;
//	font[whichFont].baseIndex = 0;

	for( size_t i = 0; i < strlen(textLine); ++i )
		{
			texture_glyph_t *glyph = texture_font_get_glyph( font[whichFont].font, textLine + i );

			if( glyph != NULL )
				{
					float kerning = 0.0f;

					if ( i > 0)
						{
							kerning = texture_glyph_get_kerning( glyph, textLine + i - 1 );
						}

					font[whichFont].currentX += kerning;

					int x0  = (int)( font[whichFont].currentX + glyph->offset_x );
					int y0  = (int)( font[whichFont].currentY + glyph->offset_y );
					int x1  = (int)( x0 + glyph->width );
					int y1  = (int)( y0 - glyph->height );

					float s0 = glyph->s0;
					float t0 = glyph->t0;
					float s1 = glyph->s1;
					float t1 = glyph->t1;

					//
					// Corner one - 0
					sdf_addVertexInfo(whichFont, glm::vec2 {x0, y0}, glm::vec2 {s0,t0}, lineColor);
					//
					// Corner two - 1
					sdf_addVertexInfo(whichFont, glm::vec2 {x0, y1}, glm::vec2 {s0,t1}, lineColor);
					//
					// Corner three - 2
					sdf_addVertexInfo(whichFont, glm::vec2 {x1, y1}, glm::vec2 {s1,t1}, lineColor);
					//
					// Corner four - 3
					sdf_addVertexInfo(whichFont, glm::vec2 {x1, y0}, glm::vec2 {s1,t0}, lineColor);

					font[whichFont].currentX += glyph->advance_x;

					vertIndex[whichFont].push_back(0 + font[whichFont].baseIndex);
					vertIndex[whichFont].push_back(1 + font[whichFont].baseIndex);
					vertIndex[whichFont].push_back(2 + font[whichFont].baseIndex);
					vertIndex[whichFont].push_back(0 + font[whichFont].baseIndex);
					vertIndex[whichFont].push_back(2 + font[whichFont].baseIndex);
					vertIndex[whichFont].push_back(3 + font[whichFont].baseIndex);

					font[whichFont].baseIndex += NUM_VERTS_PER_GLYPH;
				}
		}
}

//------------------------------------------------------------------
//
// Draw the VBO's for each of the fonts
// Check the size of the vector to see if it has been used this frame
void sdf_displayText()
//------------------------------------------------------------------
{
	for (int i = 0; i != MAX_NUM_FONTS; i++)
		{
			if (vertIndex[i].size() > 0)	// No need if there is no data t draw
				{
					//
					// Start using TTF font shader program
					GL_CHECK ( glUseProgram ( shaderProgram[SHADER_TTF_FONT].programID ) );

					wrapglEnable ( GL_BLEND );

					glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
					wrapglDisable ( GL_DEPTH_TEST );

					//
					// Bind the generated VAO
					GL_CHECK ( glBindVertexArray ( font[i].glyphVAO_ID ) );
					//
					// Bind texture if it's not already bound as current texture
					wrapglBindTexture ( GL_TEXTURE0, font[i].atlas->id );
					GL_CHECK ( glUniform1i ( shaderProgram[SHADER_TTF_FONT].inTextureUnit, 0 ) );

					GL_CHECK ( glUniform2f ( shaderProgram[SHADER_TTF_FONT].screenSizeID, (float)winWidth / 2, (float)winHeight / 2));

					//
					// Bind the vertex info
					GL_CHECK ( glBindBuffer ( GL_ARRAY_BUFFER, font[i].vertexVBO_ID ) );

					GL_CHECK ( glBufferData ( GL_ARRAY_BUFFER, sizeof(_sdf_Vertex) * fontVertex[i].size(), fontVertex[i].data(), GL_DYNAMIC_DRAW ) );

					GL_CHECK ( glVertexAttribPointer ( shaderProgram[SHADER_TTF_FONT].inVertsID, 2, GL_FLOAT, GL_FALSE, 		sizeof(_sdf_Vertex), (GLvoid*)offsetof(_sdf_Vertex, position) ) );
					GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[SHADER_TTF_FONT].inVertsID ) );
					//
					// Specify the texture info
					GL_CHECK ( glVertexAttribPointer ( shaderProgram[SHADER_TTF_FONT].inTextureCoordsID, 2, GL_FLOAT, GL_FALSE, sizeof(_sdf_Vertex), (GLvoid*)offsetof(_sdf_Vertex, textureCoord) ) );
					GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[SHADER_TTF_FONT].inTextureCoordsID ) );
					//
					// Specify the color array
					GL_CHECK ( glVertexAttribPointer ( shaderProgram[SHADER_TTF_FONT].inColorID, 4, GL_FLOAT, GL_FALSE, 		sizeof(_sdf_Vertex), (GLvoid*)offsetof(_sdf_Vertex, color) ) );
					GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[SHADER_TTF_FONT].inColorID ) );
					//
					//index buffer
					GL_CHECK ( glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, font[i].index_ID ) );
					GL_CHECK ( glBufferData ( GL_ELEMENT_ARRAY_BUFFER, vertIndex[i].size() * sizeof(unsigned int), vertIndex[i].data(), GL_DYNAMIC_DRAW ) );

					GL_CHECK ( glDrawElements ( GL_TRIANGLES, vertIndex[i].size(), GL_UNSIGNED_INT, 0 ) );

					glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0);

					vertIndex[i].clear();
					fontVertex[i].clear();
					font[i].baseIndex = 0;
				}
		}
}

//------------------------------------------------------------------
//
// Get the width of a passed in text string using passed in font
float sdf_getTextWidth(int whichFont, const char *text, ...)
//------------------------------------------------------------------
{
	va_list				args;
	char				textLine[MAX_STRING_SIZE];
	float				textWidth = 0.0f;
	//
	// get out the passed in parameters
	//
	va_start ( args, text );
	vsnprintf ( textLine, MAX_STRING_SIZE, text, args );
	va_end ( args );

	if (!fontInitDone)
		{
			sdf_initFontSystem();
		}

	for( size_t i = 0; i < strlen(textLine); ++i )
		{
			texture_glyph_t *glyph = texture_font_get_glyph( font[whichFont].font, textLine + i );

			if( glyph != NULL )
				{
					float kerning = 0.0f;

					if ( i > 0)
						{
							kerning = texture_glyph_get_kerning( glyph, textLine + i - 1 );
						}

					textWidth += kerning;
					textWidth += glyph->advance_x;
				}
		}

	return textWidth;
}

//------------------------------------------------------------------
//
// Get the width of a passed in text string using passed in font
float sdf_getTextHeight(int whichFont)
//------------------------------------------------------------------
{
	return font[whichFont].size;
}

