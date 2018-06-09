#pragma once


//-----------------------------------------------------------------------------
//
// Structure to hold image/textures to be loaded
//
//-----------------------------------------------------------------------------

typedef struct
{
	unsigned char	*imageData;			// Image Data (Up To 32 Bits)
	int				bpp;				// Image Color Depth In Bits Per Pixel
	int				width;				// Image Width
	int				height;				// Image Height
	unsigned int	texID;				// Texture ID Used To Select A Texture
	unsigned int	type;				// Image Type (GL_RGB, GL_RGBA)
	bool			loaded;				// State of the texture
	int				bspTexID;			// Index into the texture array from BSP. Will be mapped to actual texID on loading; -1 is not a BSP texture
} _Texture;

typedef struct
{
	char            fileName[TEXTURE_FILENAME_LENGTH];       // Filename for the image
} _textureNames;

extern vector<_Texture>		texturesLoaded;
extern _textureNames		textureNames[];

enum
{
	TEX_WHITE_SQUARE,
	TEX_FLARE,
	NUM_TEXTURES
};

extern int m_numOfTextures;		// The number of texture maps

// Load all the textures
bool io_loadAllTextures();

// Free memory used by texture array
void io_freeTextureArray();

// Load a texture and set it's TextureID
GLint utilLoadTexture ( const char *fileName, int bspIndex );

// Save current screen to file
void io_saveScreenToFile();

// Pass in the BSP texture index and return the OpenGL texID
int io_getGLTexID(int bspTexID);
