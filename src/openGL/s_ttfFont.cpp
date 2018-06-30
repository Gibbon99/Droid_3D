//
// Library include files
//#include <ft2build.h>
//#include FT_FREETYPE_H

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

#include "s_vsProfileLib.h"
#include "s_shaders.h"
#include "s_ttfFont.h"
#include "s_openGLWrap.h"

float				*vertBuffer = NULL;    // Make global
float				*textureBuffer = NULL;
float				*colorBuffer = NULL;

GLint 				*vertIndexes = NULL;
GLsizei 			*vertNumber = NULL;

GLuint				g_glyphVAO_ID;
GLuint				g_vertVBO_ID;
GLuint				g_texVBO_ID;
GLuint				g_colVBO_ID;

_glColor			fontColor;

int					memCharCount = 0;       // How many chars in vertex memory buffer
int					memCharSize = 0;        // How many chars will memory buffer hold
int					vertCount = 0;          // Vertex index for each glyph
int					colorCount = 0;         // Index counter for color array

int					renderText;

extern char			fontFileName[];       // Max size for font filename

float 				fontSizeSmall;
float 				fontSizeMedium;
float 				fontSizeLarge;

typedef struct
{
	float			advanceX;		// How far to move the cursor for the next character, horizontally
	float			advanceY;       // How far to move the cursor vertically
	float			texCoord[8];
	float			vertex[8];
	float			height;			// Height of the bitmap in pixels
	float			width;			// Width of the bitmap in pixels
	float			left;			// Horizontal position in pixels relative to the cursor
	float			top;			// Vertical position relative to the baseline in pixels
} _ttfGlyph;

typedef struct
{
	_ttfGlyph		glyph[NUM_CHARS];
	GLuint			texID;
	int				texWidth;
	int				texHeight;
	GLfloat			size;
} _ttfFont;

_ttfFont			ttfFont[NUM_TTF_FONTS];

FT_Library			library;        //Create a freetype library instance
FT_Face				fontInfo;       //Stores information on the loaded font
bool				usePowerOf2 = false;

//----------------------------------------------------------------------------
//
// Set the font filename from a script
bool ttf_setFontName ( std::string fontFileNameIn )
//----------------------------------------------------------------------------
{
	if ( strlen ( fontFileNameIn.c_str() ) > TTF_FILENAME_SIZE )
		{
			strcpy ( fontFileName, "Font filename too long." );
			return false;
		}

	strcpy ( fontFileName, fontFileNameIn.c_str() );

	return true;
}

//----------------------------------------------------------------------------
//
// Return power of two
int ttf_nextPow2 ( int a )
//----------------------------------------------------------------------------
{
	int rval = 1;

	while ( rval < a )
		rval <<=1;

	return rval;

}

//----------------------------------------------------------------------
//
// Add all the widths of the characters to get the desired texture width
float ttf_getCharsWidth ( float *getHeight, float *charWidth )
//----------------------------------------------------------------------
{
	int advanceX = 0;
	unsigned int maxHeight = 0;
	unsigned int charMaxWidth = 0;

	//
	// Find the char with the largest width
	for ( int c = START_CHAR; c != NUM_CHARS; c++ )
		{
			if ( FT_Load_Char ( fontInfo, c, FT_LOAD_DEFAULT | FT_LOAD_IGNORE_TRANSFORM | FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT ) )
				{
					io_logToFile ( "Error: Could not load char [ %i ]", c );
					return false;
				}

			FT_Bitmap * bitmap = &fontInfo->glyph->bitmap;

			if ( ( bitmap->width ) > charMaxWidth )
				charMaxWidth = bitmap->width;
		}

	//
	// Now generate each glyph and add up the width
	for ( int c = START_CHAR; c != NUM_CHARS; c++ )
		{
			if ( FT_Load_Char ( fontInfo, c, FT_LOAD_DEFAULT | FT_LOAD_IGNORE_TRANSFORM | FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT ) )
				{
					io_logToFile ( "Error: Could not load char [ %i ]", c );
					return false;
				}

			FT_Bitmap * bitmap = &fontInfo->glyph->bitmap;
			advanceX += charMaxWidth; //(bitmap->width + 1.0f);

			if ( bitmap->rows > maxHeight )
				maxHeight = bitmap->rows;
		}

	*getHeight = maxHeight;
	*charWidth = charMaxWidth;
	return advanceX;
}

//----------------------------------------------------------------------------
//
// Free memory used to hold vertex information for text
void ttf_freeTextMemory()
//----------------------------------------------------------------------------
{
	//
	// Free OpenGL buffers
	glDeleteBuffers ( 1, &g_vertVBO_ID );
	glDeleteBuffers ( 1, &g_texVBO_ID );
	glDeleteVertexArrays ( 1, &g_glyphVAO_ID );
	//
	// Free memory
	free ( textureBuffer );
	free ( vertBuffer );
	free ( colorBuffer );
	free ( vertIndexes );
	free ( vertNumber );
}

//----------------------------------------------------------------------------
//
// Setup memory to hold text information
void ttf_startText()
//----------------------------------------------------------------------------
{
	memCharSize = ( winWidth / conFontSize ) * ( winHeight / conFontSize );

	memCharSize *= 2;
// Allocate enough memory for a character to cover the screen
// (winWidth / fontSize) * (winHeight / fontSize)

//
// TODO: Stop doing this each frame
//
	if ( NULL == vertNumber )
		{
			vertNumber = ( GLsizei * ) malloc ( sizeof ( GLsizei ) * memCharSize );
			vertIndexes = ( GLint * ) malloc ( sizeof ( GLint ) * memCharSize );

			if ( ( NULL == vertIndexes ) || ( NULL == vertNumber ) )
				{
					io_logToFile ( "Memory error : vertIndexes" );
					sys_shutdownToSystem();
				}

			int indexCount = 4;

			vertIndexes[0] = 0;

			for ( int i = 0; i != memCharSize; i++ )
				{
					vertIndexes[i] = indexCount * i;
					vertNumber[i] = 4;
				}

			//
			// Setup the Vertex Array Object that will have the VBO's associated to it
			GL_ASSERT ( glGenVertexArrays ( 1, &g_glyphVAO_ID ) );
			//
			// Bind the vertex info
//			GL_ASSERT ( glGenBuffers ( 1, &g_vertVBO_ID ) );
			g_vertVBO_ID = wrapglGenBuffers(1, __func__);
//			GL_ASSERT ( glGenBuffers ( 1, &g_texVBO_ID ) );
			g_texVBO_ID = wrapglGenBuffers(1, __func__);
//			GL_ASSERT ( glGenBuffers ( 1, &g_colVBO_ID ) );
			g_colVBO_ID = wrapglGenBuffers(1, __func__);
			
			printf("Done setting TTF memory\n");

		}

	if ( NULL == textureBuffer )
		{
			textureBuffer = ( float * ) malloc ( ( sizeof ( float ) * NUM_VERTS ) * memCharSize );
			vertBuffer = 	( float * ) malloc ( ( sizeof ( float ) * NUM_VERTS ) * memCharSize );
			colorBuffer = 	( float * ) malloc ( ( sizeof ( float ) * NUM_VERTS ) * 4 * memCharSize ); //4 Color values for each of the 8 verts

			if ( ( NULL == vertBuffer ) || ( NULL == textureBuffer ) || ( NULL == colorBuffer ) )
				{
					io_logToFile ( "Error: Memory allocation error [ gl_startText ]" );
					return;
				}
		}

	memCharCount = 0;   // no characters in memory buffer
	vertCount = 0;
	colorCount = 0;
}


//----------------------------------------------------------------------
//
// Start TTF Library
bool ttf_initLibrary ( int fontSize, int whichFont )
//----------------------------------------------------------------------
{
	float           tmpWidth;
	float           tmpHeight;
	float           charWidth;
	float           destX;
	FT_Error        ttfError;

	//
// TODO (dberry#3#): Need to apply scaling to advance as well as blit size

	float scaleX = 1.0f;
	float scaleY = 1.0f;

	destX = 0.0f;

	conFontSize = fontSize;

	if ( FT_Init_FreeType ( &library ) )
		{
			io_logToFile ( "Could not initialize the freetype library." );
			return false;
		}

	ttfError = FT_New_Face ( library, fontFileName, 0, &fontInfo );

	if ( 0 != ttfError )
		{
			if ( FT_Err_Unknown_File_Format == ttfError )
				{
					printf ( "Error: TTF Unknown file format.\n" );
					return false;

				}
			else
				{
					printf ( "Could not load font file - [ %s ]\n", fontFileName );
					return false;
				}
		}

	if ( FT_Set_Char_Size ( fontInfo, ( fontSize * 64 ) * scaleX, ( fontSize * 64 ) * scaleY, 96, 96 ) )
		{
			io_logToFile ( "Could not set font size." );
			return false;
		}

	//
	// Now setup a texture to hold all our glyphs
	glGenTextures ( 1, &ttfFont[whichFont].texID );

	wrapglBindTexture ( GL_TEXTURE0, ttfFont[whichFont].texID );

	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	//
	// Get the width required to hold all the Characters
	tmpWidth = ttf_getCharsWidth ( &tmpHeight, &charWidth );

	//
	// See if we need to use a power of 2 texture or not
	if ( true == usePowerOf2 )
		{
			tmpWidth = ttf_nextPow2 ( tmpWidth );
			tmpHeight = ttf_nextPow2 ( tmpHeight );
		}

	ttfFont[whichFont].texWidth = tmpWidth * scaleX;
	ttfFont[whichFont].texHeight = tmpHeight * scaleY;

	charWidth *= scaleX;

	int     *blankTexture;

	blankTexture = ( int * ) malloc ( sizeof ( int ) * ( tmpWidth * tmpHeight * 4 ) );

	memset ( blankTexture, 0, ( tmpWidth * tmpHeight * 4 ) );

	// NULL means reserve texture memory, but texels are undefined
	glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA8, tmpWidth, tmpHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, &blankTexture[0] );

	free ( blankTexture );	// Free memory now that it's loaded to the card

	//
	// Now generate each glyph and upload to new texture
	for ( int c = START_CHAR; c != NUM_CHARS; c++ )
		{
			if ( FT_Load_Char ( fontInfo, c, FT_LOAD_DEFAULT | FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT ) )
				{
					io_logToFile ( "Error: Could not load char [ %i ]", c );
					return false;
				}

			FT_Bitmap * bitmap = &fontInfo->glyph->bitmap;
			FT_GlyphSlot g = fontInfo->glyph;

			GLubyte * data;
			int dsize = bitmap->width * bitmap->rows;
			data = new GLubyte[dsize];

			unsigned int x, y, i = 0;

			for ( y = 0; y < bitmap->rows; y++ )
				for ( x = 0; x < bitmap->width; x++ )
					data[x + ( bitmap->rows - 1 - y ) * bitmap->width] = bitmap->buffer[i++];

			GLint uplast;
			glGetIntegerv ( GL_UNPACK_ALIGNMENT, &uplast );

			if ( uplast != 1 )
				glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

			ttfFont[whichFont].glyph[c].advanceX = ( g->advance.x >> 6 ) * scaleX;
			ttfFont[whichFont].glyph[c].advanceY = ( g->advance.y >> 6 ) * scaleY;
			ttfFont[whichFont].glyph[c].width = bitmap->width * scaleX;
			ttfFont[whichFont].glyph[c].height = bitmap->rows * scaleY;
			ttfFont[whichFont].glyph[c].left = g->bitmap_left;
			ttfFont[whichFont].glyph[c].top = g->bitmap_top;

			glTexSubImage2D ( GL_TEXTURE_2D, 0, destX, 5 - ( ttfFont[whichFont].glyph[c].height - ttfFont[whichFont].glyph[c].top ), bitmap->width, bitmap->rows, GL_ALPHA, GL_UNSIGNED_BYTE, data );

			delete [] data;
			//
			// Lower left coord
			ttfFont[whichFont].glyph[c].vertex[0] = 0.0f;
			ttfFont[whichFont].glyph[c].vertex[1] = 0.0f;
			//
			// Top Left
			ttfFont[whichFont].glyph[c].vertex[2] = 0.0f;
			ttfFont[whichFont].glyph[c].vertex[3] = tmpHeight;
			//
			// Top Right
			ttfFont[whichFont].glyph[c].vertex[4] = charWidth;
			ttfFont[whichFont].glyph[c].vertex[5] = tmpHeight;
			//
			// Bottom Right
			ttfFont[whichFont].glyph[c].vertex[6] = charWidth;
			ttfFont[whichFont].glyph[c].vertex[7] = 0.0f;

			float texX = 1.0f / ( tmpWidth / ( charWidth ) );

			ttfFont[whichFont].glyph[c].texCoord[0] = ( c * texX );
			ttfFont[whichFont].glyph[c].texCoord[1] = 1.0f;

			ttfFont[whichFont].glyph[c].texCoord[2] = ( c * texX );
			ttfFont[whichFont].glyph[c].texCoord[3] = 0.0f;

			ttfFont[whichFont].glyph[c].texCoord[4] = ( c * texX ) + texX;
			ttfFont[whichFont].glyph[c].texCoord[5] = 0.0f;

			ttfFont[whichFont].glyph[c].texCoord[6] = ( c * texX ) + texX;
			ttfFont[whichFont].glyph[c].texCoord[7] = 1.0f;

			destX += charWidth;
		}

	return true;
}

//----------------------------------------------------------------------------
//
// Calculate the verts and textures and positions for each char in the string
void ttf_addText ( int whichFont, float startX, float startY, const char *text, ... )
//----------------------------------------------------------------------------
{
	va_list		    args;
	char		    printText[MAX_STRING_SIZE];
	static          bool errorLogged = false;

	//
	// get out the passed in parameters
	//
	va_start ( args, text );
	vsnprintf ( printText, MAX_STRING_SIZE, text, args );
	va_end ( args );

	float   advanceX = startX;  // Start at passed in position
	float   advanceY = startY;

//    logToFile("All - %s\n", printText);

	for ( int i = 0; i != ( int ) strlen ( printText ); i++ )
		{
			if ( printText[i] == '\n' ) // got a newline in string, move to next line
				{
					advanceY += conFontSize;
					advanceX = startX;
//            logToFile("addText \n [ %i ] %s", i, printText);
					//i++;
				}

			//
			// Lower left coord
			vertBuffer[vertCount] = ttfFont[whichFont].glyph[ ( int ) printText[i]].vertex[0] + advanceX;
			textureBuffer[vertCount++] = ttfFont[whichFont].glyph[ ( int ) printText[i]].texCoord[0];
			colorBuffer[colorCount++] = fontColor.red;   // Red
			colorBuffer[colorCount++] = fontColor.green;   // Green
			colorBuffer[colorCount++] = fontColor.blue;   // Blue
			colorBuffer[colorCount++] = fontColor.alpha;   // Alpha

			vertBuffer[vertCount] = ttfFont[whichFont].glyph[ ( int ) printText[i]].vertex[1] + advanceY;
			textureBuffer[vertCount++] = ttfFont[whichFont].glyph[ ( int ) printText[i]].texCoord[1];
			colorBuffer[colorCount++] = fontColor.red;   // Red
			colorBuffer[colorCount++] = fontColor.green;   // Green
			colorBuffer[colorCount++] = fontColor.blue;   // Blue
			colorBuffer[colorCount++] = fontColor.alpha;   // Alpha

			//
			// Top Left
			vertBuffer[vertCount] = ttfFont[whichFont].glyph[ ( int ) printText[i]].vertex[2] + advanceX;
			textureBuffer[vertCount++] = ttfFont[whichFont].glyph[ ( int ) printText[i]].texCoord[2];
			colorBuffer[colorCount++] = fontColor.red;   // Red
			colorBuffer[colorCount++] = fontColor.green;   // Green
			colorBuffer[colorCount++] = fontColor.blue;   // Blue
			colorBuffer[colorCount++] = fontColor.alpha;   // Alpha

			vertBuffer[vertCount] = ttfFont[whichFont].glyph[ ( int ) printText[i]].vertex[3] + advanceY;
			textureBuffer[vertCount++] = ttfFont[whichFont].glyph[ ( int ) printText[i]].texCoord[3];
			colorBuffer[colorCount++] = fontColor.red;   // Red
			colorBuffer[colorCount++] = fontColor.green;   // Green
			colorBuffer[colorCount++] = fontColor.blue;   // Blue
			colorBuffer[colorCount++] = fontColor.alpha;   // Alpha

			//
			// Top Right
			vertBuffer[vertCount] = ttfFont[whichFont].glyph[ ( int ) printText[i]].vertex[4] + advanceX;
			textureBuffer[vertCount++] = ttfFont[whichFont].glyph[ ( int ) printText[i]].texCoord[4];
			vertBuffer[vertCount] = ttfFont[whichFont].glyph[ ( int ) printText[i]].vertex[5] + advanceY;
			textureBuffer[vertCount++] = ttfFont[whichFont].glyph[ ( int ) printText[i]].texCoord[5];
			//
			// Bottom Right
			vertBuffer[vertCount] = ttfFont[whichFont].glyph[ ( int ) printText[i]].vertex[6] + advanceX;
			textureBuffer[vertCount++] = ttfFont[whichFont].glyph[ ( int ) printText[i]].texCoord[6];
			vertBuffer[vertCount] = ttfFont[whichFont].glyph[ ( int ) printText[i]].vertex[7] + advanceY;
			textureBuffer[vertCount++] = ttfFont[whichFont].glyph[ ( int ) printText[i]].texCoord[7];

			advanceX += ttfFont[whichFont].glyph[ ( int ) printText[i]].advanceX;

			if ( memCharCount >= memCharSize )  // Used all memory up
				{
					if ( false == errorLogged )
						{
							errorLogged = true;
							io_logToFile ( "Info: Out of memory to hold characters" );
							return;
						}
				}
			memCharCount++;
		}
}

//----------------------------------------------------------------------------
//
// Display all the text in memory buffers
void ttf_displayText ( int whichFont )
//----------------------------------------------------------------------------
{
	//
	// Start using TTF font shader program
	GL_CHECK ( glUseProgram ( shaderProgram[SHADER_TTF_FONT].programID ) );
	//
	// Bind the generated VAO
	GL_CHECK ( glBindVertexArray ( g_glyphVAO_ID ) );

	wrapglEnable ( GL_BLEND );

	glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	wrapglDisable ( GL_DEPTH_TEST );
//
// Bind texture if it's not already bound as current texture
	wrapglBindTexture ( GL_TEXTURE0, ttfFont[whichFont].texID );
	GL_CHECK ( glUniform1i ( shaderProgram[SHADER_TTF_FONT].inTextureUnit, 0 ) );
//
// Bind the vertex info
	GL_CHECK ( glBindBuffer ( GL_ARRAY_BUFFER, g_vertVBO_ID ) );
	GL_CHECK ( glBufferData ( GL_ARRAY_BUFFER, sizeof ( GLfloat ) * NUM_VERTS * memCharSize, vertBuffer, GL_DYNAMIC_DRAW ) );
	GL_CHECK ( glVertexAttribPointer ( shaderProgram[SHADER_TTF_FONT].inVertsID, 2, GL_FLOAT, false, 0, BUFFER_OFFSET ( 0 ) ) );
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[SHADER_TTF_FONT].inVertsID ) );
//
// Bind the texture coordinates
	GL_CHECK ( glBindBuffer ( GL_ARRAY_BUFFER, g_texVBO_ID ) );
	GL_CHECK ( glBufferData ( GL_ARRAY_BUFFER, sizeof ( GLfloat ) * NUM_VERTS * memCharSize, textureBuffer, GL_DYNAMIC_DRAW ) );
	GL_CHECK ( glVertexAttribPointer ( shaderProgram[SHADER_TTF_FONT].inTextureCoordsID, 2, GL_FLOAT, false, 0, BUFFER_OFFSET ( 0 ) ) );
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[SHADER_TTF_FONT].inTextureCoordsID ) );
//
// Bind color array
	GL_CHECK ( glBindBuffer ( GL_ARRAY_BUFFER, g_colVBO_ID ) );
	GL_CHECK ( glBufferData ( GL_ARRAY_BUFFER, sizeof ( GLfloat ) * NUM_VERTS * 4 * memCharSize, colorBuffer, GL_DYNAMIC_DRAW ) );
	GL_CHECK ( glVertexAttribPointer ( shaderProgram[SHADER_TTF_FONT].inColorID, 4, GL_FLOAT, false, 0, BUFFER_OFFSET ( 0 ) ) );
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[SHADER_TTF_FONT].inColorID ) );
//
// Move to start of screen
	modelMatrix = glm::translate ( glm::mat4(), glm::vec3 ( 0.0f, 0.0f, -1.0 ) );
//
// Load the matrixes into the vertex shader
	GL_CHECK ( glUniformMatrix4fv ( shaderProgram[SHADER_TTF_FONT].modelMat, 1, false, glm::value_ptr ( modelMatrix ) ) );
//
// viewMatrix is causing flickering on the 3d screen
	GL_CHECK ( glUniformMatrix4fv ( shaderProgram[SHADER_TTF_FONT].viewProjectionMat, 1, false, glm::value_ptr ( projMatrix * glm::mat4() ) ) );

	{
//        PROFILE("glMultiDrawArrays");
		switch ( renderText )
			{
			case TEXT_MULTI_ARRAY:
				GL_CHECK ( glMultiDrawArrays ( GL_TRIANGLE_FAN,  vertIndexes,  vertNumber, memCharCount ) );
				break;

			case TEXT_DRAW_ARRAY:
				for ( int i = 0; i != memCharCount; i++ )
					GL_CHECK ( glDrawArrays ( GL_TRIANGLE_FAN, vertIndexes[i], vertNumber[i] ) );
				break;
			}
	}

	glBindBuffer ( GL_ARRAY_BUFFER, 0 );
	glBindVertexArray ( 0 );

	wrapglDisable ( GL_BLEND );

	memCharCount = 0;
	colorCount = 0;
}

//-----------------------------------------------------------------------------
//
// Return the texture ID for a passed in font
int ttf_returnTexID ( int whichFont )
//-----------------------------------------------------------------------------

{
	return ttfFont[whichFont].texID;
}

//-----------------------------------------------------------------------------
//
// Set the font color
void ttf_setFontColor ( float red, float green, float blue, float alpha )
//-----------------------------------------------------------------------------
{
	fontColor.red = red;
	fontColor.green = green;
	fontColor.blue = blue;
	fontColor.alpha = alpha;
}
