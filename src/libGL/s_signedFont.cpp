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
//	vector<_sdf_Vertex>		vertex;
//	vector<uint>			index;
	texture_font_t			*font;
	texture_atlas_t			*atlas;
};

_font						font[MAX_NUM_FONTS];

vector<_sdf_Vertex>			font0Vert;
vector<_sdf_Vertex>			font1Vert;
vector<_sdf_Vertex>			font2Vert;
vector<unsigned int>		vert0Index;
vector<unsigned int>		vert1Index;
vector<unsigned int>		vert2Index;

const char * cache = " !\"#$%&'()*+,-./0123456789:;<=>?"
                     "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
                     "`abcdefghijklmnopqrstuvwxyz{|}~";

texture_atlas_t 	*sdf_Atlas;
texture_font_t		*sdf_Font = 0;

GLuint				sdf_g_glyphVAO_ID;
GLuint				sdf_g_index_ID;
GLuint				sdf_g_vertexVBO_ID;

vector<_sdf_Vertex>			sdf_vertex;
vector<unsigned int>		sdf_indexes;


//------------------------------------------------------------------
//
// Add information for font
void sdf_addFontInfo(uint whichFont, string fileName, uint fontSize)
//------------------------------------------------------------------
{
	font[whichFont].fileName = fileName;
	font[whichFont].size = fontSize;
	font[whichFont].isLoaded = false;
//	font[whichFont].vertex.clear();
//	font[whichFont].index.clear();
//	font[whichFont].font = NULL;
//	font[whichFont].atlas = NULL;
	font[whichFont].baseIndex = 0;
	font[whichFont].currentX = 0.0f;
	font[whichFont].currentY = 0.0f;
	font[whichFont].isAvailable = io_doesFileExist(fileName);

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

	switch (whichFont)
		{
			case 0:
				font0Vert.push_back(sdf_vertexTemp);
				break;

			case 1:
				font1Vert.push_back(sdf_vertexTemp);
				break;

			case 2:
				font2Vert.push_back(sdf_vertexTemp);
				break;
		}
}

//------------------------------------------------------------------
//
// Populate the temporary vertex and upload to array
void sdf_addVertexInfo_2(glm::vec2 position, glm::vec2 textureCoord, glm::vec4 color)
//------------------------------------------------------------------
{
	_sdf_Vertex		sdf_vertexTemp;				// Temp to hold vertex info before adding to vector

	sdf_vertexTemp.position = position;
	sdf_vertexTemp.textureCoord = textureCoord;
	sdf_vertexTemp.color = color;
	sdf_vertex.push_back(sdf_vertexTemp);
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

					switch (whichFont)
						{
							case 0:
								vert0Index.push_back(0 + font[whichFont].baseIndex);
								vert0Index.push_back(1 + font[whichFont].baseIndex);
								vert0Index.push_back(2 + font[whichFont].baseIndex);
								vert0Index.push_back(0 + font[whichFont].baseIndex);
								vert0Index.push_back(2 + font[whichFont].baseIndex);
								vert0Index.push_back(3 + font[whichFont].baseIndex);
								break;

							case 1:
								vert1Index.push_back(0 + font[whichFont].baseIndex);
								vert1Index.push_back(1 + font[whichFont].baseIndex);
								vert1Index.push_back(2 + font[whichFont].baseIndex);
								vert1Index.push_back(0 + font[whichFont].baseIndex);
								vert1Index.push_back(2 + font[whichFont].baseIndex);
								vert1Index.push_back(3 + font[whichFont].baseIndex);
								break;

							case 2:
								vert2Index.push_back(0 + font[whichFont].baseIndex);
								vert2Index.push_back(1 + font[whichFont].baseIndex);
								vert2Index.push_back(2 + font[whichFont].baseIndex);
								vert2Index.push_back(0 + font[whichFont].baseIndex);
								vert2Index.push_back(2 + font[whichFont].baseIndex);
								vert2Index.push_back(3 + font[whichFont].baseIndex);
								break;
						}

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



// check index size to proceed or not

	for (int i = 0; i != MAX_NUM_FONTS; i++)
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

			switch (i)
				{
					case 0:
						GL_CHECK ( glBufferData ( GL_ARRAY_BUFFER, sizeof(_sdf_Vertex) * font0Vert.size(), &font0Vert[0].position, GL_DYNAMIC_DRAW ) );
						break;

					case 1:
						GL_CHECK ( glBufferData ( GL_ARRAY_BUFFER, sizeof(_sdf_Vertex) * font1Vert.size(), &font1Vert[0].position, GL_DYNAMIC_DRAW ) );
						break;

					case 2:
						GL_CHECK ( glBufferData ( GL_ARRAY_BUFFER, sizeof(_sdf_Vertex) * font2Vert.size(), &font2Vert[0].position, GL_DYNAMIC_DRAW ) );
						break;
				}

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

			switch (i)
				{
					case 0:
						GL_CHECK ( glBufferData ( GL_ELEMENT_ARRAY_BUFFER, vert0Index.size() * sizeof(unsigned int), &vert0Index[0], GL_DYNAMIC_DRAW ) );
						GL_CHECK ( glDrawElements ( GL_TRIANGLES, vert0Index.size(), GL_UNSIGNED_INT, 0 ) );
						vert0Index.clear();
						font0Vert.clear();
						break;

					case 1:
						GL_CHECK ( glBufferData ( GL_ELEMENT_ARRAY_BUFFER, vert1Index.size() * sizeof(unsigned int), &vert1Index[0], GL_DYNAMIC_DRAW ) );
						GL_CHECK ( glDrawElements ( GL_TRIANGLES, vert1Index.size(), GL_UNSIGNED_INT, 0 ) );
						vert1Index.clear();
						font1Vert.clear();
						break;


					case 2:
						GL_CHECK ( glBufferData ( GL_ELEMENT_ARRAY_BUFFER, vert2Index.size() * sizeof(unsigned int), &vert2Index[0], GL_DYNAMIC_DRAW ) );
						GL_CHECK ( glDrawElements ( GL_TRIANGLES, vert2Index.size(), GL_UNSIGNED_INT, 0 ) );
						vert2Index.clear();
						font2Vert.clear();
						break;
				}
		}
}




//------------------------------------------------------------------
//
// Init the signed distance font
void sdf_printText(glm::vec2 position, glm::vec4 lineColor, const char *text, ... )
//------------------------------------------------------------------
{
	static bool					initDone = false;
	const char 					*sdf_Filename = "data/oldDigital.ttf";

	float						currentX, currentY;			// Current location for the glyph on screen
	unsigned int				baseIndex = 0;				// Starting index for indexed drawing - points to vertexes to make triangles

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

	sdf_vertex.clear();
	sdf_indexes.clear();

	if (false == initDone)
		{
			sdf_Atlas = texture_atlas_new( 512, 512, 1);
			sdf_Font = texture_font_new_from_file (sdf_Atlas, 24, sdf_Filename);

			if (NULL == sdf_Font)
				{
					con_print(CON_INFO, true, "ERROR: Could not start SDF Fonts. Missing file [ %s ]", sdf_Filename);
					return;
				}

			sdf_Font->rendermode = RENDER_SIGNED_DISTANCE_FIELD;

			texture_font_load_glyphs( sdf_Font, cache );

			glGenTextures( 1, &sdf_Atlas->id );
			glBindTexture( GL_TEXTURE_2D, sdf_Atlas->id );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, sdf_Atlas->width, sdf_Atlas->height, 0, GL_RED, GL_UNSIGNED_BYTE, sdf_Atlas->data );

			//
			// Setup the Vertex Array Object that will have the VBO's associated to it
			GL_ASSERT ( glGenVertexArrays ( 1, &sdf_g_glyphVAO_ID ) );

			GL_ASSERT ( glGenBuffers ( 1, &sdf_g_index_ID ) );

			GL_ASSERT ( glGenBuffers ( 1, &sdf_g_vertexVBO_ID) );

			initDone = true;
		}

	for( size_t i = 0; i < strlen(textLine); ++i )
		{
			texture_glyph_t *glyph = texture_font_get_glyph( sdf_Font, textLine + i );

			if( glyph != NULL )
				{
					float kerning = 0.0f;

					if ( i > 0)
						{
							kerning = texture_glyph_get_kerning( glyph, textLine + i - 1 );
						}

					currentX += kerning;

					int x0  = (int)( currentX + glyph->offset_x );
					int y0  = (int)( currentY + glyph->offset_y );
					int x1  = (int)( x0 + glyph->width );
					int y1  = (int)( y0 - glyph->height );

					float s0 = glyph->s0;
					float t0 = glyph->t0;
					float s1 = glyph->s1;
					float t1 = glyph->t1;

					//
					// Corner one - 0
					sdf_addVertexInfo_2(glm::vec2 {x0, y0}, glm::vec2 {s0,t0}, lineColor);
					//
					// Corner two - 1
					sdf_addVertexInfo_2(glm::vec2 {x0, y1}, glm::vec2 {s0,t1}, lineColor);
					//
					// Corner three - 2
					sdf_addVertexInfo_2(glm::vec2 {x1, y1}, glm::vec2 {s1,t1}, lineColor);
					//
					// Corner four - 3
					sdf_addVertexInfo_2(glm::vec2 {x1, y0}, glm::vec2 {s1,t0}, lineColor);

					currentX += glyph->advance_x;

					sdf_indexes.push_back(0 + baseIndex);
					sdf_indexes.push_back(1 + baseIndex);
					sdf_indexes.push_back(2 + baseIndex);
					sdf_indexes.push_back(0 + baseIndex);
					sdf_indexes.push_back(2 + baseIndex);
					sdf_indexes.push_back(3 + baseIndex);

					baseIndex += NUM_VERTS_PER_GLYPH;
				}
		}

	//
	// Now upload and draw the line of text
	//

	//
	// Start using TTF font shader program
	GL_CHECK ( glUseProgram ( shaderProgram[SHADER_TTF_FONT].programID ) );
	//
	// Bind the generated VAO
	GL_CHECK ( glBindVertexArray ( sdf_g_glyphVAO_ID ) );

	wrapglEnable ( GL_BLEND );

	glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	wrapglDisable ( GL_DEPTH_TEST );
	//
	// Bind texture if it's not already bound as current texture
	wrapglBindTexture ( GL_TEXTURE0, sdf_Atlas->id );
	GL_CHECK ( glUniform1i ( shaderProgram[SHADER_TTF_FONT].inTextureUnit, 0 ) );

	GL_CHECK ( glUniform2f ( shaderProgram[SHADER_TTF_FONT].screenSizeID, (float)winWidth / 2, (float)winHeight / 2));

	//
	// Bind the vertex info
	GL_CHECK ( glBindBuffer ( GL_ARRAY_BUFFER, sdf_g_vertexVBO_ID ) );
	GL_CHECK ( glBufferData ( GL_ARRAY_BUFFER, sizeof(_sdf_Vertex) * sdf_vertex.size(), &sdf_vertex[0].position, GL_DYNAMIC_DRAW ) );
	GL_CHECK ( glVertexAttribPointer ( shaderProgram[SHADER_TTF_FONT].inVertsID, 2, GL_FLOAT, GL_FALSE, sizeof(_sdf_Vertex), (GLvoid*)offsetof(_sdf_Vertex, position)) );
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[SHADER_TTF_FONT].inVertsID ) );
	//
	// Specify the texture info
	GL_CHECK ( glVertexAttribPointer ( shaderProgram[SHADER_TTF_FONT].inTextureCoordsID, 2, GL_FLOAT, GL_FALSE, sizeof(_sdf_Vertex), (GLvoid*)offsetof(_sdf_Vertex, textureCoord) ) );
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[SHADER_TTF_FONT].inTextureCoordsID ) );
	//
	// Specify the color array
	GL_CHECK ( glVertexAttribPointer ( shaderProgram[SHADER_TTF_FONT].inColorID, 4, GL_FLOAT, GL_FALSE, sizeof(_sdf_Vertex), (GLvoid*)offsetof(_sdf_Vertex, color) ) );
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[SHADER_TTF_FONT].inColorID ) );
	//
	//index buffer
	GL_CHECK ( glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, sdf_g_index_ID ) );
	GL_CHECK ( glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sdf_indexes.size() * sizeof(unsigned int), &sdf_indexes[0], GL_DYNAMIC_DRAW ) );

	GL_CHECK ( glDrawElements ( GL_TRIANGLES, sdf_indexes.size(), GL_UNSIGNED_INT, 0 ) );
}
