#include "SOIL.h"
#include "s_globals.h"
#include "io_textures.h"
#include "s_openGL.h"
#include "io_loadImage.h"
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

vector<_Texture>		texturesLoaded;

//-----------------------------------------------------------------------------
//
// Structure to hold image/textures to be loaded
//
//-----------------------------------------------------------------------------

// bpp, width, height, texID, type, loaded, fileName

_textureNames		textureNames[] =  	// holds all the information about a texture
{
	{"white_square"},
	{"flare"},
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
int loadImageFile ( char *fileName, int bspIndex )
//-----------------------------------------------------------------------------
{
	_Texture	tempTexture;

	stbi_uc     *imageBuffer = nullptr;
	int         imageLength;

	//
	// Setup initial value for each slot
	tempTexture.loaded = false;

	imageLength = io_getFileSize ( fileName );

	if ( imageLength < 0 )
		return -1;

	imageBuffer = ( stbi_uc * ) malloc ( sizeof ( char ) * imageLength );

	if ( nullptr == imageBuffer )
		return -1;

	if ( -1 == io_getFileIntoMemory ( fileName, ( char * ) imageBuffer ) )
	{
		if (imageBuffer != nullptr)
			free(imageBuffer);
		return -1;
	}

	tempTexture.texID = SOIL_load_OGL_texture_from_memory ( imageBuffer, imageLength, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_TEXTURE_REPEATS );
	if (tempTexture.texID == 0) // failed to load texture
	{
		tempTexture.loaded = false;
		if (imageBuffer != nullptr)
			free(imageBuffer);
		return -1;
	}

	tempTexture.loaded = true;
	tempTexture.bspTexID = bspIndex;

	free ( imageBuffer );

	texturesLoaded.push_back ( tempTexture );

	return tempTexture.texID;
}

//-----------------------------------------------------------------------------
//
// Load a texture and set it's TextureID
GLint utilLoadTexture ( const char *fileName, int bspIndex )
//-----------------------------------------------------------------------------
{
	//
	// TODO: Something is passing invalid filename - bspIndex is -1 - must be from model
	//
	char          tempFileName[MAX_STRING_SIZE];
	GLint         returnTexID = 0;    // Not GLUint - as it returns -1 on error

	if ( strlen ( fileName ) < 1 )
		return -1;		//TODO: Models passing in invalid filename??
		
	if (strcmp(fileName, "Material load error") == 1)
	{
		con_print(CON_INFO, true, "ERROR: Could not load material for model [ %s ]", fileName);
		return -1;
	}

	for ( int i = 0; i != NUM_SUPPORTED_FILES; i++ )
		{
			if ( strstr ( fileName, fileTypes[i].fileType ) != NULL )
				{
					// Filename already has the extension on it - so load it
					returnTexID = loadImageFile ( ( char * ) fileName, bspIndex );

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

					returnTexID = loadImageFile ( tempFileName, bspIndex );

					if ( returnTexID != -1 )
						{
							if ( true == verbose )
								con_print ( CON_TEXT, true, "Loaded texture [ %s ] - ID [ %i ]", tempFileName, returnTexID );

							return returnTexID;
						}
				}
		}

	con_print ( CON_ERROR, true, "ERROR: File not found or not supported - [ %s ]", fileName );
	return -1;
}

//-----------------------------------------------------------------------------
//
// Free memory used by texture array
void io_freeTextureArray()
//-----------------------------------------------------------------------------
{
	texturesLoaded.clear();
}

//-----------------------------------------------------------------------------
//
// Load all the textures from the BSP and defined
bool io_loadAllTextures()
//-----------------------------------------------------------------------------
{

	for ( int i = 0; i != NUM_TEXTURES; i++ )
		{
			utilLoadTexture ( textureNames[i].fileName, -1 );
		}

	for ( int i = 0; i != m_numOfTextures; i++ )
		{
			utilLoadTexture ( m_pTextures[i].strName, i );
		}

	RET_TRUE ( "Texture loading complete.", true );
}

//-----------------------------------------------------------------------------
//
// Pass in the BSP texture index and return the OpenGL texID
int io_getGLTexID ( int bspTexID )
//-----------------------------------------------------------------------------
{
	for ( unsigned int i = 0; i != texturesLoaded.size(); i++ )
		{
			if ( bspTexID == texturesLoaded[i].bspTexID )
				return texturesLoaded[i].texID;
		}
	con_print(CON_ERROR, true, "Could not find matching texture for id [ %i ]", bspTexID);
	return -1;
}
