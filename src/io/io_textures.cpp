#include "s_globals.h"
#include "io_textures.h"
#include "s_openGL.h"
#include "s_loadImage.h"
#include "io_fileSystem.h"
#include "s_openGLWrap.h"
#include "FreeImage.h"
#include "s_varsBSP.h"

typedef struct
{
	char	fileType[5];
} _fileTypes;

_fileTypes	fileTypes[] =
{
	{ ".bmp" },
	{ ".jpg" },
	{ ".tga" },
	{ ".png" }
};

#define NUM_SUPPORTED_FILES 4

_Texture     *texturesLoaded = NULL;        // Unknown number of textures to be loaded from BSP info

//-----------------------------------------------------------------------------
//
// Structure to hold image/textures to be loaded
//
//-----------------------------------------------------------------------------

// bpp, width, height, texID, type, loaded, fileName

_textureNames		textureNames[] =  	// holds all the information about a texture
{
	{"wall"},
	{"triax_tracks"},
	{"alltiles"},
	{"white_square"},
};

//-----------------------------------------------------------------------------
//
// Save current screen to file
void io_saveScreenToFile()
//-----------------------------------------------------------------------------
{

	glBindFramebuffer ( GL_FRAMEBUFFER, 0 );

	// Make the BYTE array, factor of 3 because it's RBG.
	BYTE* pixels = new BYTE[ 3 * winWidth * winHeight];

	glReadPixels ( 0, 0, winWidth, winHeight, GL_BGR, GL_UNSIGNED_BYTE, pixels );

	// Convert to FreeImage format & save to file
	FIBITMAP* image = FreeImage_ConvertFromRawBits ( pixels, winWidth, winHeight, 3*winWidth, 24, 0x00FF00, 0xFF0000, 0x0000FF, false );
	FreeImage_Save ( FIF_PNG, image, "test.png", 0 );

	// Free resources
	FreeImage_Unload ( image );
	delete [] pixels;
}


//-----------------------------------------------------------------------------
//
// Load a image from disk/archive file and convert to a OpenGL texture
int loadImageFile ( int whichTexture, char *fileName, int bspIndex )
//-----------------------------------------------------------------------------
{
	stbi_uc     *imageBuffer;
	int         imageLength;

	//
	// Setup initial value for each slot
	texturesLoaded[whichTexture].loaded = false;

	imageLength = io_getFileSize ( fileName );

	if ( imageLength < 0 )
		return -1;

	imageBuffer = ( stbi_uc * ) malloc ( sizeof ( char ) * imageLength );

	if ( NULL == imageBuffer )
		return -1;

	if ( -1 == io_getFileIntoMemory ( fileName, ( char * ) imageBuffer ) )
		return -1;

	texturesLoaded[whichTexture].imageData = stbi_load_from_memory ( imageBuffer, imageLength, &texturesLoaded[whichTexture].width, &texturesLoaded[whichTexture].height, &texturesLoaded[whichTexture].bpp, 0 );

	if ( !texturesLoaded[whichTexture].imageData )
		{
			texturesLoaded[whichTexture].loaded = false;
			return -1;
		}

	texturesLoaded[whichTexture].loaded = true;

	texturesLoaded[whichTexture].bspTexID = bspIndex;
	//
	// Create an OpenGL texture for the image
	GL_CHECK ( glGenTextures ( 1, &texturesLoaded[whichTexture].texID ) );

	wrapglBindTexture ( GL_TEXTURE0, texturesLoaded[whichTexture].texID );
	// Set texture parameters
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
//
// GL_SRGB is here instead of GL_RGB to change gamma corrected textures back to linear color space
// TODO (dberry#1#): Put in paramter for textures/shaders not doing gamma correction
//
	glTexImage2D ( GL_TEXTURE_2D, 0, GL_SRGB, texturesLoaded[whichTexture].width, texturesLoaded[whichTexture].height, 0, GL_RGB, GL_UNSIGNED_BYTE, texturesLoaded[whichTexture].imageData );
	free ( imageBuffer );
	// Free imagedata as well?

	return texturesLoaded[whichTexture].texID;
}

//-----------------------------------------------------------------------------
//
// Load a texture and set it's TextureID
GLint utilLoadTexture ( GLuint whichTexture, const char *fileName, int bspIndex)
//-----------------------------------------------------------------------------
{
	char          tempFileName[MAX_STRING_SIZE];
	GLint         returnTexID = 0;    // Not GLUint - as it returns -1 on error

	for ( int i = 0; i != NUM_SUPPORTED_FILES; i++ )
		{
			if ( strstr ( fileName, fileTypes[i].fileType ) != NULL )
				{
					// Filename already has the extension on it - so load it
					returnTexID = loadImageFile ( whichTexture, ( char * ) fileName, bspIndex );

					if ( returnTexID != -1 )
						{
							if ( true == verbose )
								con_print ( CON_TEXT, true, "Loaded texture [ %s ] - ID [ %i ]", fileName, returnTexID );

							return returnTexID;
						}
				}

			else
				{
					// Filename doesn't have an extension on it
					strcpy ( tempFileName, "" );
					strcpy ( tempFileName, fileName );
					strcat ( tempFileName, fileTypes[i].fileType );

					returnTexID = loadImageFile ( whichTexture, tempFileName, bspIndex );

					if ( returnTexID != -1 )
						{
							if ( true == verbose )
								con_print ( CON_TEXT, true, "Loaded texture [ %s ] - ID [ %i ]", tempFileName, returnTexID );

							return returnTexID;
						}
				}
		}

	con_printUpdate ( true, "File not found or not supported - [ %s ]", fileName );
	return -1;
}

//-----------------------------------------------------------------------------
//
// Setp the memory to hold info for textures for the models
bool io_setupModelTextureMemory ( int numberTextures )
//-----------------------------------------------------------------------------
{
	if ( !texturesLoaded )
		RET_FALSE ( "Texture memory not allocated yet - call before loading models.", "" );

	texturesLoaded = ( _Texture * ) realloc ( texturesLoaded, sizeof ( _Texture ) * ( numberTextures + NUM_TEXTURES ) );

	if ( NULL == texturesLoaded )
		RET_FALSE ( "Memory reallocation error for model textures", "" );

	return true;
}

//-----------------------------------------------------------------------------
//
// Setup the memory to hold info for all the loaded textures
bool utilSetupTextureMemory ( int numberTextures )
//-----------------------------------------------------------------------------
{
	if ( texturesLoaded )
		{
			con_print ( CON_TEXT, true, "Memory already allocated for texturesLoaded. Freeing..." );
			free ( texturesLoaded );
		}

	if ( numberTextures < 1 )
		RET_FALSE ( "Invalid number of textures", true );

	texturesLoaded = ( _Texture * ) malloc ( sizeof ( _Texture ) * numberTextures );

	if ( NULL == texturesLoaded )
		RET_FALSE ( "Memory allocation error. [ %s ]", __FILE__ );

	//
	// Now clear out the structure
	for ( int i = 0; i != numberTextures; i++ )
		{
			texturesLoaded[i].imageData = NULL;
			texturesLoaded[i].bpp = 0;
			texturesLoaded[i].width = 0;
			texturesLoaded[i].height = 0;
			texturesLoaded[i].type = 0;
			texturesLoaded[i].texID = -1;
			texturesLoaded[i].loaded = false;
			texturesLoaded[i].bspTexID = -1;
		}

	RET_TRUE ( "Memory allocated for textures.", 0 );
}

//-----------------------------------------------------------------------------
//
// Free memory used by texture array
void io_freeTextureArray()
//-----------------------------------------------------------------------------
{
	if ( texturesLoaded )
		free ( texturesLoaded );
}

//-----------------------------------------------------------------------------
//
// Load all the textures from the BSP and defined
bool io_loadAllTextures()
//-----------------------------------------------------------------------------
{
	int bspTextureCount = 0;

	if ( false == utilSetupTextureMemory ( NUM_TEXTURES + m_numOfTextures ) )
		return false;

	for ( int i = 0; i != NUM_TEXTURES; i++ )
		{
			utilLoadTexture ( i, textureNames[i].fileName, -1 );
		}

	for (int i = NUM_TEXTURES; i != NUM_TEXTURES + m_numOfTextures; i++)
		{
			utilLoadTexture (i, m_pTextures[bspTextureCount].strName, bspTextureCount );
			bspTextureCount++;
		}

	RET_TRUE ( "Texture loading complete.", true );
}

//-----------------------------------------------------------------------------
//
// Pass in the BSP texture index and return the OpenGL texID
int io_getGLTexID(int bspTexID)
//-----------------------------------------------------------------------------
{
	for (int i = 0; i != NUM_TEXTURES + m_numOfTextures; i++)
	{
		if (bspTexID = texturesLoaded[i].bspTexID)
			return texturesLoaded[i].texID;
	}
}
